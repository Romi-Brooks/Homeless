/**
  * @file           : MetadataReader.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/10/12
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_METADATAREADER_HPP
#define HOMELESS_METADATAREADER_HPP

#include <string>
#include <iostream>
#include <taglib/taglib.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>

namespace engine::audio::metadata {
	class MetadataReader {
		private:

		public:
			static auto GetMetadata(const std::string& musicPath) -> void {
				// 打开音频文件
				const TagLib::FileRef file(musicPath.c_str());

				// 检查文件是否有效
				if (!file.isNull() && file.tag()) {
					const TagLib::Tag* tag = file.tag();

					// 读取元数据
					std::cout << "=== 音频文件信息 ===" << std::endl;
					std::cout << "文件路径: " << musicPath << std::endl;
					std::cout << "标题: " << tag->title().toCString(true) << std::endl;
					std::cout << "艺术家: " << tag->artist().toCString(true) << std::endl;
					std::cout << "专辑: " << tag->album().toCString(true) << std::endl;
					std::cout << "年份: " << tag->year() << std::endl;
					std::cout << "音轨号: " << tag->track() << std::endl;
					std::cout << "流派: " << tag->genre().toCString(true) << std::endl;
					std::cout << "注释: " << tag->comment().toCString(true) << std::endl;
				} else {
					std::cerr << "无法打开文件或文件不包含元数据: " << musicPath << std::endl;
				}
			}
	};
}


#endif // HOMELESS_METADATAREADER_HPP
