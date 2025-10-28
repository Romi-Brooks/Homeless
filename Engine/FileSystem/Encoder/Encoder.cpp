/**
  * @file           : Encoder.cpp
  * @author         : Romi Brooks
  * @brief          : The Filename Encoding Checker
  * @attention      : Ensure filenames are normal UTF-8 content for 3rd party/native libraries to work with
  * @date           : 2025/10/12
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/
#include <vector>

#include "utf8.h"
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>  // 用于Windows平台的编码转换
#endif

#include "Encoder.hpp"

namespace engine::filesystem {
	bool Encoder::isUtf8Valid(const std::string& utf8Str) {
		return utf8::is_valid(utf8Str.begin(), utf8Str.end());
	}
	std::string toNative(const std::string& utf8Str) {
		if (!Encoder::isUtf8Valid(utf8Str)) {
			throw std::invalid_argument("Invalid UTF-8 string: " + utf8Str);
		}

		#ifdef _WIN32
				// 废弃原 CP_ACP 转换，直接返回 UTF-8 字符串（后续用宽字符 API 访问文件）
				// 注：这里返回 UTF-8 只是为了兼容接口，实际文件访问会用 toWidePath 获取宽字符路径
				return utf8Str;
		#else
				// Linux/macOS 本地编码就是 UTF-8，直接返回
				return utf8Str;
		#endif
	}

	#ifdef _WIN32
	std::wstring Encoder::toWidePath(const std::string& utf8Str) {
		if (!isUtf8Valid(utf8Str)) {
			throw std::invalid_argument("Invalid UTF-8 string for wide path: " + utf8Str);
		}

		std::vector<wchar_t> utf16Vec = toUtf16(utf8Str);
		return std::wstring(utf16Vec.begin(), utf16Vec.end());
	}
	#endif
	std::string Encoder::fromNative(const std::string& nativeStr) {
#ifdef _WIN32
		// Windows系统：本地ANSI -> UTF-16 -> UTF-8
		int wideSize = MultiByteToWideChar(
			CP_ACP,               // 系统默认ANSI代码页
			0,                    // 转换标志
			nativeStr.c_str(),    // 输入本地字符串
			static_cast<int>(nativeStr.size()),  // 输入长度
			nullptr,              // 输出缓冲区（先获取大小）
			0                     // 输出缓冲区大小
		);

		if (wideSize == 0) {
			throw std::runtime_error("Failed to convert native encoding to UTF-8");
		}

		// 转换为UTF-16
		std::vector<wchar_t> utf16Str(wideSize);
		if (MultiByteToWideChar(
			CP_ACP,
			0,
			nativeStr.c_str(),
			static_cast<int>(nativeStr.size()),
			utf16Str.data(),
			wideSize
		) == 0) {
			throw std::runtime_error("Failed to convert native encoding to UTF-8");
		}

		// 转换为UTF-8
		return fromUtf16(utf16Str);
#else
		// Linux/macOS系统：本地编码即为UTF-8，直接返回
		return nativeStr;
#endif
	}
	std::vector<wchar_t> Encoder::toUtf16(const std::string& utf8Str) {
		if (!isUtf8Valid(utf8Str)) {
			throw std::invalid_argument("Invalid UTF-8 string");
		}

		std::vector<wchar_t> utf16Str;
		utf8::utf8to16(utf8Str.begin(), utf8Str.end(), std::back_inserter(utf16Str));
		return utf16Str;
	}
	std::string Encoder::fromUtf16(const std::vector<wchar_t>& utf16Str) {
		std::string utf8Str;
		utf8::utf16to8(utf16Str.begin(), utf16Str.end(), std::back_inserter(utf8Str));
		return utf8Str;
	}
	size_t Encoder::getCharacterCount(const std::string& utf8Str) {
		if (!isUtf8Valid(utf8Str)) {
			throw std::invalid_argument("Invalid UTF-8 string");
		}

		return utf8::distance(utf8Str.begin(), utf8Str.end());
	}
}

