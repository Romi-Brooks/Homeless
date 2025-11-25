#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <limits>
#include <sstream>
#include <algorithm>

#ifdef _WIN32
#include "windows.h"
#endif // _WIN32

// 引入你的打包/解包头文件
#include "Packager.hpp"
#include "Unpackager.hpp"

// 命名空间别名
namespace fs = std::filesystem;
using engine::tools::package::Packager;
using engine::tools::unpackage::Unpackager;

// 清空输入缓冲区
auto ClearInputBuffer() -> void {
    std::cin.clear();  // 清除错误状态
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// 递归遍历单个目录，收集所有文件路径
std::vector<std::string> TraverseSingleDirectory(const std::string& dir_path) {
    std::vector<std::string> file_paths;
    try {
        if (!fs::exists(dir_path)) {
            std::cerr << "[Warning] Directory does not exist:" << dir_path << std::endl;
            return file_paths;
        }
        if (!fs::is_directory(dir_path)) {
            std::cerr << "[Warning] Not a valid directory:" << dir_path << std::endl;
            return file_paths;
        }

        // 递归遍历目录，仅收集普通文件
        for (const auto& entry : fs::recursive_directory_iterator(dir_path)) {
            if (fs::is_regular_file(entry)) {
                file_paths.push_back(entry.path().string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "[Error] Directory traversal failed:" << dir_path << " -> " << e.what() << std::endl;
    }
    return file_paths;
}

// 遍历多个目录，收集所有文件路径
std::vector<std::string> TraverseMultipleDirectories(const std::vector<std::string>& dir_paths) {
    std::vector<std::string> all_file_paths;
    for (const auto& dir : dir_paths) {
        auto single_dir_files = TraverseSingleDirectory(dir);
        // 将单个目录的文件路径合并到总列表
        all_file_paths.insert(all_file_paths.end(), single_dir_files.begin(), single_dir_files.end());
    }
    return all_file_paths;
}

// 解析用户输入的多目录
std::vector<std::string> ParseMultiDirectories(const std::string& input) {
    std::vector<std::string> dir_paths;
    std::stringstream ss(input);
    std::string dir;

    // 按逗号分割输入字符串
    while (std::getline(ss, dir, ',')) {
        // 去除目录名前后的空格：res1/, res2/
        dir.erase(dir.begin(), std::find_if(dir.begin(), dir.end(), [](unsigned char c) {
            return !std::isspace(c);
        }));
        dir.erase(std::find_if(dir.rbegin(), dir.rend(), [](unsigned char c) {
            return !std::isspace(c);
        }).base(), dir.end());

        if (!dir.empty()) {
            dir_paths.push_back(dir);
        }
    }
    return dir_paths;
}

// 打包函数
auto PackFiles(const std::string& packName, const std::vector<std::string>& resourcePath) {
    Packager packer;
    Packager::Config config;
    config.verbose = true;

    std::cout << "Commencing packing... Total number of files awaiting packing:" << resourcePath.size() << std::endl;
    const auto result = packer.Pack(resourcePath, packName, config);

    if (result == Packager::Result::SUCCESS) {
        std::cout << "Packing successful!" << std::endl;
        packer.PrintPackageInfo();
        return true;
    } else {
        std::cout << "Packing failed!" << std::endl;
        return false;
    }
}

// 原有解包函数
auto UnpackAllToFolder(const std::string& packName) -> bool {
    Unpackager unpacker;
    if (unpacker.Load(packName) == Unpackager::Result::SUCCESS) {
        unpacker.PrintPackageInfo();
        // 解包到磁盘
        Unpackager::Config unpackConfig;
        unpackConfig.verbose = true;
        unpackConfig.outputDir = "extract/";
        unpackConfig.preserveStructure = true;
        unpacker.UnpackAll(unpackConfig);

    	std::cout << "Unpacking successful!" << std::endl;
    	return true;
    } else {
    	std::cout << "Unpacking failed!" << std::endl;
    	return false;
    }
}

auto main() -> int {
	#ifdef _WIN32
		SetConsoleOutputCP(CP_UTF8);
	#endif // _WIN32

    std::cout << "================================================" << std::endl;
    std::cout << "Simple Resource Package / Unpackage Tools v1.0  " << std::endl;
    std::cout << "Use For Homeless                                " << std::endl;
    std::cout << "================================================" << std::endl;

    int choice = 0;
    while (true) {
        // 显示菜单
        std::cout << "\nOperations：" << std::endl;
        std::cout << "1. Pack from multiple target folders" << std::endl;
        std::cout << "2. Extract to the extract folder" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Input options (0-2):";

        // 处理用户输入（防止非数字输入）
        if (!(std::cin >> choice)) {
            ClearInputBuffer();
            std::cerr << "Error: Please enter a valid number (0-2)!" << std::endl;
            continue;
        }
        ClearInputBuffer();  // 清空缓冲区，避免影响后续字符串输入

        // 根据选择执行功能
        switch (choice) {
            case 1: {
                std::string dir_input, pack_name;
                std::cout << "\nTips: Enter multiple directories separated by commas (e.g.:resources/,audio/,textures/):" << std::endl;
                std::cout << "Please enter the directory paths to be packed:";
                std::getline(std::cin, dir_input);
                std::cout << "Please enter the output package name (e.g. media_res.dat):";
                std::getline(std::cin, pack_name);

                // 1. 解析用户输入的多目录
                std::vector<std::string> dir_paths = ParseMultiDirectories(dir_input);
                if (dir_paths.empty()) {
                    std::cerr << "Error: No valid directories entered!" << std::endl;
                    break;
                }
                std::cout << "Successfully parsed " << dir_paths.size() << " directories to pack." << std::endl;

                // 2. 遍历多目录，收集所有文件路径
                std::vector<std::string> all_files = TraverseMultipleDirectories(dir_paths);
                if (all_files.empty()) {
                    std::cerr << "Error: No files found in the entered directories!" << std::endl;
                    break;
                }

                // 3. 调用打包函数
                PackFiles(pack_name, all_files);
                break;
            }
            case 2: {
                std::string pack_name;
                std::cout << "\nPlease enter the name of the package file to be unpacked (e.g., media_res.dat):";
                std::getline(std::cin, pack_name);
                UnpackAllToFolder(pack_name);
                break;
            }
            case 0: {
                std::cout << "\nExited. Goodbye!" << std::endl;
                return 0;
            }
            default:
                std::cerr << "Error: Invalid option, please enter a number between 0 and 2!" << std::endl;
                break;
        }

        std::cout << "\n-------------------------------------" << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.get();
    }
}
