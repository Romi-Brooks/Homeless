// Standard Library
#include <algorithm>
#include <iostream>

// Third party Library
#include <utf8.h>

// Self Dependencies
#include "Packager.hpp"

namespace engine::tools::package {
	Packager::Config::Config() : compress(false), preserveStructure(true), overwrite(true) {}

	std::string Packager::NormalizePath(const std::string& path) {
	    try {
	        std::string result = path;
	        std::ranges::replace(result, '\\', '/');
	        if (result.size() >= 2 && result[0] == '.' && result[1] == '/') {
	            result = result.substr(2);
	        }
	        return result;
	    } catch (...) {
	        return path;
	    }
	}

	bool Packager::IsValidUTF8(const std::string& str) {
	    try {
	        return utf8::is_valid(str.begin(), str.end());
	    } catch (...) {
	        return false;
	    }
	}

	std::string Packager::ToUTF8(const std::string& str) {
	    try {
	        if (!IsValidUTF8(str)) {
	            std::string temp;
	            utf8::replace_invalid(str.begin(), str.end(), std::back_inserter(temp));
	            return temp;
	        }
	        return str;
	    } catch (...) {
	        return "invalid_encoding_file";
	    }
	}

	std::string Packager::SafePathToString(const fs::path& path) {
	    try {
	        return path.string();
	    } catch (const std::exception& e) {
	    	std::cout << "Warning: Path conversion error: " << e.what() << std::endl;
	        return "unknown_path";
	    }
	}

	std::string Packager::SafeRelativePath(const fs::path& path) {
	    try {
	        return fs::relative(path).string();
	    } catch (const std::exception& e) {
	    	std::cout << "Warning: Unable to obtain relative path: " << e.what() << std::endl;
	        return path.filename().string();
	    }
	}

	bool Packager::CollectFiles(const std::vector<std::string>& resourcePaths, std::vector<fs::path>& allFiles, const Config& config) {
	    for (const auto& path_str : resourcePaths) {
	        try {
	            fs::path path(path_str);
	            if (!fs::exists(path)) {
	            	std::cout << "Warning: Path does not exist: " << path_str << std::endl;
	                continue;
	            }

	            if (fs::is_directory(path)) {
	                for (const auto& entry : fs::recursive_directory_iterator(path)) {
	                    if (entry.is_regular_file()) {
	                        allFiles.push_back(entry.path());
	                    }
	                }
	            } else if (fs::is_regular_file(path)) {
	                allFiles.push_back(path);
	            }
	        } catch (const std::exception& e) {
	            std::cout << "Warning: Error occurred while processing the path " << path_str << ": " << e.what() << std::endl;
	        }
	    }
	    return !allFiles.empty();
	}

	std::string Packager::GenerateInternalFilename(const fs::path& filePath, const Config& config) {
	    try {
	        if (config.preserveStructure) {
				const std::string relative_path = SafeRelativePath(filePath);
	            return ToUTF8(NormalizePath(relative_path));
	        } else {
				const std::string filename = ToUTF8(SafePathToString(filePath.filename()));
	            int counter = 1;
	            std::string final_name = filename;

	            while (file_index_.contains(final_name)) {
	                std::string stem = ToUTF8(SafePathToString(filePath.stem()));
					const std::string extension = ToUTF8(SafePathToString(filePath.extension()));
	                final_name = stem + "_" + std::to_string(counter) += extension;
	                counter++;
	            }
	            return final_name;
	        }
	    } catch (const std::exception& e) {
	    	std::cout << "Error: Failed to generate internal filename: " << e.what() << std::endl;
	        return "unknown_file_" + std::to_string(file_index_.size());
	    }
	}

	Packager::Result Packager::Pack(const std::vector<std::string>& resourcePaths, const std::string& outputFile, const Config& config) {
	    file_table_.clear();
	    file_index_.clear();

	    try {
	        if (fs::exists(outputFile)) {
	            if (!config.overwrite) {
	                return Result::ERROR_WRITE_FAILED;
	            }
	            fs::remove(outputFile);
	        }
	    } catch (const std::exception& e) {
	    	std::cout << "Error: Unable to delete existing file: " << e.what() << std::endl;
	        return Result::ERROR_WRITE_FAILED;
	    }

	    std::vector<fs::path> allFiles;
	    if (!CollectFiles(resourcePaths, allFiles, config)) {
	        return Result::ERROR_EMPTY_PACKAGE;
	    }

	    std::ofstream output(outputFile, std::ios::binary);
	    if (!output.is_open()) {
	        return Result::ERROR_OPEN_OUTPUT;
	    }

	    // 写入文件头
	    output.write(MAGIC, 4);
	    output.write(reinterpret_cast<const char*>(&VERSION), sizeof(VERSION));

	    // 预留文件表位置 - 先写入文件数量为0，稍后更新
	    uint64_t file_table_offset = output.tellp();
	    uint32_t file_count = 0;
	    output.write(reinterpret_cast<const char*>(&file_count), sizeof(file_count));

	    // 写入文件数据
	    uint64_t current_offset = file_table_offset + sizeof(file_count);
	    std::vector<FileEntry> entries;

	    for (const auto& filePath : allFiles) {
	        std::ifstream input;
	        try {
	            input.open(filePath, std::ios::binary);
	        } catch (const std::exception& e) {
	        	std::cout << "Warning: Unable to open file: " << SafePathToString(filePath)
	        			  << " - " << e.what() << std::endl;
	            continue;
	        }

	        if (!input.is_open()) {
	        	std::cout << "Warning: Unable to open file: " << SafePathToString(filePath) << std::endl;
	            continue;
	        }

	        // 获取文件大小
	        input.seekg(0, std::ios::end);
	        uint64_t file_size = static_cast<uint64_t>(input.tellg());
	        input.seekg(0, std::ios::beg);

	        if (file_size == 0) {
	        	std::cout << "Warning: Skipping empty files: " << SafePathToString(filePath) << std::endl;
	            input.close();
	            continue;
	        }

	        // 创建文件条目
	        FileEntry entry;
	        try {
	            entry.original_path = SafePathToString(filePath);
	            entry.filename = GenerateInternalFilename(filePath, config);
	            entry.type = ToUTF8(SafePathToString(filePath.extension()));
	            entry.offset = current_offset;
	            entry.size = file_size;
	        } catch (const std::exception& e) {
	        	std::cout << "Error: Failed to create file entry: " << e.what() << std::endl;
	            input.close();
	            continue;
	        }

	        // 读取并写入文件数据
	        std::vector<char> buffer(file_size);
	        input.read(buffer.data(), static_cast<long long>(file_size));

	        if (!input) {
	        	std::cout << "Warning: Failed to read file: " << SafePathToString(filePath) << std::endl;
	            input.close();
	            continue;
	        }
	        input.close();

	        output.write(buffer.data(), static_cast<long long>(file_size));
	        if (!output) {
	            return Result::ERROR_WRITE_FAILED;
	        }

	        current_offset += file_size;
	        entries.push_back(entry);
	        file_count++;

	    	std::cout << "Packing: " << entry.filename << " (" << file_size << " bytes)" << std::endl;
	    }

	    if (file_count == 0) {
	        output.close();
	        try {
	            fs::remove(outputFile);
	        } catch (...) {

	        }
	        return Result::ERROR_EMPTY_PACKAGE;
	    }

	    // 记录文件表开始位置
	    uint64_t table_start = output.tellp();

	    // 更新文件计数
	    output.seekp(static_cast<std::streampos>(static_cast<long long>(file_table_offset)));
	    output.write(reinterpret_cast<const char*>(&file_count), sizeof(file_count));
	    output.seekp(static_cast<std::streampos>(static_cast<long long>(table_start)));

	    // 写入每个文件条目
	    for (const auto& entry : entries) {
	        // 写入文件名
	        std::string safe_filename = ToUTF8(entry.filename);
	        auto name_length = static_cast<uint16_t>(safe_filename.length());
	        output.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length));
	        output.write(safe_filename.c_str(), name_length);

	        // 写入文件信息
	        uint64_t offset = entry.offset;
	        uint64_t size = entry.size;
	        output.write(reinterpret_cast<const char*>(&offset), sizeof(offset));
	        output.write(reinterpret_cast<const char*>(&size), sizeof(size));

	        // 写入文件类型
	        std::string safe_type = ToUTF8(entry.type);
	        auto type_length = static_cast<uint8_t>(safe_type.length());
	        output.write(reinterpret_cast<const char*>(&type_length), sizeof(type_length));
	        output.write(safe_type.c_str(), type_length);

	        // 保存到内部文件表
	        file_table_.push_back(entry);
	        file_index_[entry.filename] = file_table_.size() - 1;
	    }

	    // 写入文件表偏移量
	    uint64_t table_end = static_cast<uint64_t>(output.tellp());
	    output.write(reinterpret_cast<const char*>(&table_start), sizeof(table_start));

	    output.close();

	    // 验证包文件
		std::cout << "Packing complete, commencing verification..." << std::endl;
		std::ifstream verify(outputFile, std::ios::binary);
		if (verify.is_open()) {
			char magic[4];
			verify.read(magic, 4);
			std::string magic_str(magic, 4);
			std::cout << "Verification magic number: " << magic_str << " " << (magic_str == "HPKG" ? "T" : "F") << std::endl;

			uint16_t version;
			verify.read(reinterpret_cast<char*>(&version), sizeof(version));
			std::cout << "Verification version: " << version << " " << (version == 1 ? "T" : "F") << std::endl;

			uint32_t file_count_verify;
			verify.read(reinterpret_cast<char*>(&file_count_verify), sizeof(file_count_verify));
			std::cout << "Number of documents to be verified: " << file_count_verify << " " << (file_count_verify == file_count ? "T" : "F") << std::endl;

			// 读取文件表偏移量
			verify.seekg(-static_cast<std::streamoff>(sizeof(uint64_t)), std::ios::end);
			uint64_t table_offset_verify;
			verify.read(reinterpret_cast<char*>(&table_offset_verify), sizeof(table_offset_verify));
			std::cout << "Verification file table offset: " << table_offset_verify << " "
					  << (table_offset_verify == table_start ? "T" : "F") << std::endl;

			verify.close();
	    }

		std::cout << "Packing completed: " << outputFile << std::endl;
		std::cout << "Includes "<< file_count << " files" << std::endl;
		std::cout << "Package size: " << table_end + sizeof(uint64_t) << " bytes" << std::endl;

	    return Result::SUCCESS;
	}

	std::vector<std::string> Packager::GetPackedFiles() const {
	    std::vector<std::string> files;
	    for (const auto& entry : file_table_) {
	        files.push_back(entry.filename);
	    }
	    return files;
	}

	void Packager::PrintPackageInfo() const {
	    std::cout << "The package contains " << file_table_.size() << " files:" << std::endl;
	    for (const auto& entry : file_table_) {
	        std::cout << "  " << entry.filename << " [" << entry.type << "] - "
	                  << entry.size << " bytes" << std::endl;
	    }
	}
}
