/**
  * @file           : Encoder.hpp
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

#ifndef HOMELESS_ENCODER_HPP
#define HOMELESS_ENCODER_HPP

// Standard Lib
#include <string>

namespace engine::filesystem {
	class Encoder {
		public:
			// Check if the file directory is in plain English
			static bool IsPureAscii(const std::string& path);

			// Convert U8 strings to U16
			static std::wstring u8tou16(const std::string& u8);
	};
}



#endif // HOMELESS_ENCODER_HPP
