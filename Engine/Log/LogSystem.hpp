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
	ATMOS_INFO,
	ATMOS_WARNING,
	ATMOS_ERROR,
	ATMOS_DEBUG
};

enum class LogChannel {
	ENGINE_PHYSICS_EDGE_DETECT,
	ENGINE_MOVEMENT,

	ENGINE_AUDIO_MUSIC,
	ENGINE_AUDIO_SFX,
	ENGINE_AUDIO_MUSIC_FADE,

	ENGINE_WINDOW,
	ENGINE_SCREEN,
	ENGINE_SCREEN_MANAGER,

	GAME_MAIN,
};

class Log {
private:
	Log() = default;
	~Log() = default;


	LogLevel ViewLogLevel = LogLevel::ATMOS_INFO;
	std::mutex LogMutex;

public:
	static Log& GetLogInstance();

	static void LogOut(LogChannel channel, LogLevel level, const std::string& logMessage);
	static void SetViewLogLevel(LogLevel ViewLogLevel);

	Log(const Log&) = delete;
	Log& operator=(const Log&) = delete;
};

#define LOG_INFO(channel,logMessage) Log::GetLogInstance().LogOut(channel, LogLevel::ATMOS_INFO, logMessage)
#define LOG_WARNING(channel,logMessage) Log::GetLogInstance().LogOut(channel, LogLevel::ATMOS_WARNING, logMessage)
#define LOG_ERROR(channel,logMessage) Log::GetLogInstance().LogOut(channel, LogLevel::ATMOS_ERROR, logMessage)
#define LOG_DEBUG(channel,logMessage) Log::GetLogInstance().LogOut(channel, LogLevel::ATMOS_DEBUG, logMessage)

#endif //LOG_HPP