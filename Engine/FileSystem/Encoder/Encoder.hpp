/**
  * @file           : Encoder.hpp
  * @author         : Romi Brooks
  * @brief          : The Filename Encoding Checker
  * @attention      : Ensure filenames are normal UTF-8 content for 3rd party/native libraries to work with
  * @date           : 2025/10/12
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_ENCODER_HPP
#define HOMELESS_ENCODER_HPP

// Standard Lib
#include <string>

namespace engine::filesystem {
	class Encoder {
		public:
			/**
			 * @brief 验证UTF-8字符串是否合法
			 * @param utf8Str UTF-8字符串
			 * @return 合法返回true，否则返回false
			 */
			static bool isUtf8Valid(const std::string& utf8Str);

			/**
			 * @brief 将UTF-8字符串转换为系统本地编码（用于文件系统访问）
			 * @param utf8Str UTF-8字符串
			 * @return 系统本地编码的字符串
			 * @throw std::invalid_argument 当输入不是合法UTF-8时
			 */
			static std::string toNative(const std::string& utf8Str);

			/**
			 * @brief 将系统本地编码字符串转换为UTF-8
			 * @param nativeStr 系统本地编码的字符串
			 * @return UTF-8字符串
			 */
			static std::string fromNative(const std::string& nativeStr);

			/**
			 * @brief 将UTF-8字符串转换为UTF-16宽字符串
			 * @param utf8Str UTF-8字符串
			 * @return UTF-16宽字符串
			 * @throw std::invalid_argument 当输入不是合法UTF-8时
			 */
			static std::vector<wchar_t> toUtf16(const std::string& utf8Str);

			/**
			 * @brief 将UTF-16宽字符串转换为UTF-8
			 * @param utf16Str UTF-16宽字符串
			 * @return UTF-8字符串
			 */
			static std::string fromUtf16(const std::vector<wchar_t>& utf16Str);

			/**
			 * @brief 计算UTF-8字符串的字符数（不是字节数）
			 * @param utf8Str UTF-8字符串
			 * @return 字符数量
			 * @throw std::invalid_argument 当输入不是合法UTF-8时
			 */
			static size_t getCharacterCount(const std::string& utf8Str);

			#ifdef _WIN32
					static std::wstring toWidePath(const std::string& utf8Str);
			#endif

		private:
			// 禁止实例化，所有方法都是静态的
			Encoder() = delete;
			~Encoder() = delete;
	};
}



#endif // HOMELESS_ENCODER_HPP
