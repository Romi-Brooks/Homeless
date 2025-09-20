/*  Copyright (c) 2025 Romi Brooks <qq1694821929@gmail.com>
*  File Name: LogSystem.hpp
 *  Lib: Beeplayer I/O Log System definitions
 *  Author: Romi Brooks
 *  Date: 2025-04-24
 *  Type: I/O, LOG System
 */

#ifndef LOG_HPP
#define LOG_HPP

#include <mutex>

enum class LogLevel {
	HL_INFO,
	HL_WARNING,
	HL_ERROR,
	HL_DEBUG
};

enum class LogChannel {
	ENGINE_EDGE_DETECT,
	ENGINE_AUDIO_MUSIC,
	ENGINE_AUDIO_SFX,
};

class Log {
private:
	Log() = default;
	~Log() = default;


	LogLevel ViewLogLevel = LogLevel::HL_INFO;
	std::mutex LogMutex;

public:
	static Log& GetLogInstance();

	static void LogOut(const std::string& I_LogMessage, LogLevel LogLevel = LogLevel::HL_INFO);
	static void SetViewLogLevel(LogLevel ViewLogLevel);

	Log(const Log&) = delete;
	Log& operator=(const Log&) = delete;
};

#define LOG_INFO(LogMessage) Log::GetLogInstance().LogOut(LogMessage, BP_INFO)
#define LOG_WARNING(LogMessage) Log::GetLogInstance().LogOut(LogMessage, BP_WARNING)
#define LOG_ERROR(LogMessage) Log::GetLogInstance().LogOut(LogMessage, BP_ERROR)
#define LOG_DEBUG(LogMessage) Log::GetLogInstance().LogOut(LogMessage, BP_DEBUG)

#endif //LOG_HPP