/**
  * @file           : Encoder.cpp
  * @author         : Romi Brooks
  * @brief          : The Filename Encoding Checker
  * @attention      : Ensure filenames are normal UTF-8 content for 3rd party/native libraries to work with
  * @date           : 2025/10/12
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

// We need to know that in Windows, there are many encoding formats like:
// UTF-8 (normally used now), or GBK (In some Chinese's Windows os, the terminal use this encoding), etc.
// These diverse encoding formats cause many problems. For example, when you try to play a Chinese or Japanese song,
// As you can imagine, if your file's encoding is UTF-8 and your application is also UTF-8,
// but your terminal uses GBK encoding. it cannot display the correct content. this can lead to the "mojibake" problem.
// The output being garbled is probably the least of the issues.
// When you try to load a file using the UTF-8 encoding format,
// but the load function does not process the open logic correctly
// (like if your ANSI code page is GBK, which does not include all the characters in UTF-8)
// It can cause loading issues that are hard to resolve.

// Standard Lib
#include <vector>

// Self Dependencies
#include "Encoder.hpp"
#include "../Log/LogSystem.hpp"

bool engine::filesystem::Encoder::IsPureAscii(const std::string& path) {
	for (const char c : path) {
		// 检查字符是否为ASCII（0-127）
		if (!isascii(static_cast<unsigned char>(c))) {
			return false;
		}
	}
	return true;
}

std::wstring engine::filesystem::Encoder::u8tou16(const std::string& u8)  {
	std::vector<uint16_t> utf16_buf; // 存储UTF-16代码单元
	for (size_t i = 0; i < u8.size();) {
		uint32_t cp = 0; // Unicode代码点
		const auto lead = static_cast<uint8_t>(u8[i]);

		// 解析UTF-8序列
		if (lead < 0x80) {
			cp = lead;
			i += 1;
		} else if ((lead >> 5) == 0x6) {
			cp = (lead & 0x1F) << 6 | (u8[i+1] & 0x3F);
			i += 2;
		} else if ((lead >> 4) == 0xE) {
			cp = (lead & 0x0F) << 12 | (u8[i+1] & 0x3F) << 6 | (u8[i+2] & 0x3F);
			i += 3;
		} else if ((lead >> 3) == 0x1E) {
			cp = (lead & 0x07) << 18 | (u8[i+1] & 0x3F) << 12 | (u8[i+2] & 0x3F) << 6 | (u8[i+3] & 0x3F);
			i += 4;
		} else {
			LOG_ERROR(log::LogChannel::ENGINE_FILESYSTEM_ENCODER,"Error UTF-8 Sequence for file name: " + u8);
		}

		// 转换为UTF-16
		if (cp <= 0xFFFF) {
			utf16_buf.push_back(static_cast<uint16_t>(cp));
		} else {
			cp -= 0x10000;
			utf16_buf.push_back(static_cast<uint16_t>(0xD800 | (cp >> 10)));
			utf16_buf.push_back(static_cast<uint16_t>(0xDC00 | (cp & 0x3FF)));
		}
	}
	LOG_ERROR(log::LogChannel::ENGINE_FILESYSTEM_ENCODER, "Converted the file:" + u8 + " from u8 to u16!");
	// 转换为wchar_t（自动适应平台）
	return std::basic_string<wchar_t>(utf16_buf.begin(), utf16_buf.end());
}
