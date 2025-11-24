#include <iostream>

#include "Packager.hpp"
#include "Unpackager.hpp"

using engine::tools::package::Packager;
using engine::tools::unpackage::Unpackager;
// 打包示例
auto PackFiles(const std::string& packName, const std::vector<std::string>& resourcePath) {
	Packager packer;
	Packager::Config config;
	config.verbose = true;

	std::cout << "开始打包..." << std::endl;
	const auto result = packer.Pack(resourcePath, packName, config);

	if (result == Packager::Result::SUCCESS) {
		std::cout << "打包成功!" << std::endl;
		packer.PrintPackageInfo();
		return true;
	} else {
		std::cout << "打包失败!" << std::endl;
		return false;
	}
}

// 解包示例
auto UnpackAllToFolder(const std::string& packName) {
	Unpackager unpacker;
	if (unpacker.Load(packName) == Unpackager::Result::SUCCESS) {
		unpacker.PrintPackageInfo();
		// 解包到磁盘
		Unpackager::Config unpackConfig;
		unpackConfig.verbose = true;
		unpackConfig.outputDir = "Extracted/";
		unpackConfig.preserveStructure = true;
		unpacker.UnpackAll(unpackConfig);
	}
	return Unpackager::Result::SUCCESS;
}

auto UnpackAllToMemory(const std::string& packName) {
	Unpackager unpacker;
	if (unpacker.Load(packName) == Unpackager::Result::SUCCESS) {
		unpacker.PrintPackageInfo();
	}
	return Unpackager::Result::SUCCESS;
}

auto UnpackFileToMemory(const std::string& packName, const std::string& fileName) {
	Unpackager unpacker;
	if (unpacker.Load(packName) == Unpackager::Result::SUCCESS) {
		unpacker.PrintPackageInfo();

		// 使用MemoryFile结构
		Unpackager::MemoryFile memoryFile;
		if (unpacker.ExtractFileToMemory(fileName, memoryFile) == Unpackager::Result::SUCCESS) {
			std::cout << "成功加载文件到内存: " << memoryFile.filename << " (" << memoryFile.GetSize() << " 字节)" << std::endl;
			return memoryFile;
		}

		// // 方式2: 使用简化版本
		// auto memoryFile = unpacker.ExtractFileToMemory(fileName);

		// // 方式3: 直接获取数据指针
		// const char* data;
		// size_t size;
		// if (unpacker.GetFileData(fileName, &data, &size) == Unpackager::Result::SUCCESS) {
		// 	return data, size;
		// }
	}
}

auto main() -> int {
	std::cout << "Hello!" << std::endl;
}