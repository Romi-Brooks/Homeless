/**
  * @file           : LogSystem.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/20
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef LOG_HPP
#define LOG_HPP

#include <mutex>
namespace engine::log {
	enum class LogLevel {
		ATMOS_INFO,
		ATMOS_WARNING,
		ATMOS_ERROR,
		ATMOS_DEBUG
	};
	enum class LogChannel {
		ENGINE_PHYSICS_EDGE_DETECT,
		ENGINE_CONFIG_MOVEMENT,

		ENGINE_AUDIO_MUSIC,
		ENGINE_AUDIO_SFX,
		ENGINE_AUDIO_PLUG_MUSIC_FADE,

		ENGINE_WINDOW,
		ENGINE_SCREEN,
		ENGINE_SCREEN_MANAGER,

		ENGINE_FILESYSTEM_ENCODER,

		ENGINE_GAME_NPC,
		ENGINE_GAME_PLAYER,
		ENGINE_GAME_SCREEN,

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
			static void SetViewLogLevel(LogLevel viewLogLevel);

			Log(const Log&) = delete;
			Log& operator=(const Log&) = delete;
	};
}

#define LOG_INFO(channel,logMessage) engine::log::Log::GetLogInstance().LogOut(channel, engine::log::LogLevel::ATMOS_INFO, logMessage)
#define LOG_WARNING(channel,logMessage) engine::log::Log::GetLogInstance().LogOut(channel, engine::log::LogLevel::ATMOS_WARNING, logMessage)
#define LOG_ERROR(channel,logMessage) engine::log::Log::GetLogInstance().LogOut(channel, engine::log::LogLevel::ATMOS_ERROR, logMessage)
#define LOG_DEBUG(channel,logMessage) engine::log::Log::GetLogInstance().LogOut(channel, engine::log::LogLevel::ATMOS_DEBUG, logMessage)

#endif //LOG_HPP