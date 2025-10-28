/**
  * @file           : Unpackage.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/26
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_UNPACKAGE_HPP
#define HOMELESS_UNPACKAGE_HPP


#include <string>
#include <vector>
#include <unordered_map>
#include "../../FileSystem/Encoder.hpp"

// 和Package一致的文件条目结构（存储解析后的元信息）


class Unpackage {
	struct FileEntry {
		std::string filename;  // UTF-8编码的文件名（和Package内部存储一致）
		size_t offset;         // 文件在PAK内存缓冲区中的偏移
		size_t size;           // 文件大小
		std::string type;      // 文件类型（可选）
	};
	private:
		std::string m_pakPath;          // PAK文件路径（仅记录，不持有句柄）
		std::vector<char> m_pakBuffer;  // 存储整个PAK的内存缓冲区
		std::unordered_map<std::string, FileEntry> m_fileEntries;  // UTF-8路径→条目
		bool m_isLoaded;                // 是否已成功加载并解析

		// 辅助函数：验证PAK格式合法性（标识、文件数、条目范围）
		bool validatePakFormat() const;
		// 核心函数：解析PAK中的文件条目（填充m_fileEntries）
		bool parseFileEntries();

	public:
		Unpackage();
		~Unpackage();

		// 加载并解析PAK文件（成功返回true，失败返回false）
		bool load(const std::string& pakPath);
		// 卸载内存中的PAK数据（释放缓冲区和条目）
		void unload();

		// 状态查询：是否已加载
		bool isLoaded() const;
		// 状态查询：是否包含指定UTF-8路径的文件
		bool hasFile(const std::string& filenameUtf8) const;
		// 状态查询：获取所有文件的UTF-8路径列表
		std::vector<std::string> getFileList() const;

		// 核心功能：提取文件数据（返回指针+大小，供SFML loadFromMemory直接使用）
		// 注意：返回的指针指向m_pakBuffer，卸载前有效
		bool getFileData(const std::string& filenameUtf8, const char*& outData, size_t& outSize) const;
		// 重载：提取文件数据到用户提供的vector（拷贝一份，卸载后仍有效）
		bool getFileData(const std::string& filenameUtf8, std::vector<char>& outBuffer) const;
};



#endif // HOMELESS_UNPACKAGE_HPP
