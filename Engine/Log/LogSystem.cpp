/**
  * @file           : LogSystem.cpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/20
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

// Standard Library
#include <string>
#include <iostream>
#include <mutex>
#include <chrono>
#include <sstream>
#include <iomanip>

// Self Dependencies
#include "LogSystem.hpp"

using engine::log::Log;
using engine::log::LogLevel;
using engine::log::LogChannel;

// Forward Function
std::string GetLogLevel(const LogLevel& logLevel) {
    switch (logLevel) {
        case LogLevel::ATMOS_INFO: return "INFO";
        case LogLevel::ATMOS_WARNING: return"WARNING";
        case LogLevel::ATMOS_ERROR: return "ERROR";
        case LogLevel::ATMOS_DEBUG: return "DEBUG";
    }
	return "Call func with error level.";
}

std::string GetLogChannel(const LogChannel& channel) {
	switch (channel) {
		case LogChannel::ENGINE_AUDIO_MUSIC: return "Engine.Audio.Music -> ";
		case LogChannel::ENGINE_AUDIO_SFX: return"Engine.Audio.SFX -> ";
		case LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE: return "Engine.Audio.Music.Fade -> ";

		case LogChannel::ENGINE_CONFIG_MOVEMENT: return "Engine.Movement -> ";

		case LogChannel::ENGINE_ENTITY:return "Engine.Entity -> ";
		case LogChannel::ENGINE_ENTITY_NPC: return "Engine.Entity.NPC -> ";
		case LogChannel::ENGINE_ENTITY_PLAYER: return "Engine.Entity.Player -> ";

		case LogChannel::ENGINE_SCREEN: return  "Engine.Screen -> ";
		case LogChannel::ENGINE_SCREEN_MANAGER: return "Engine.Screen.Manager -> ";

		case LogChannel::ENGINE_WINDOW: return "Engine.Window -> ";

		case LogChannel::ENGINE_PHYSICS_EDGE_DETECT: return "Engine.Physics.EdgeDetect -> ";

		case LogChannel::ENGINE_FILESYSTEM_ENCODER: return "Engine.Filesystem.Encoder -> ";

		case LogChannel::ENGINE_LUA: return "Engine.Lua -> ";

		case LogChannel::GAME_NPC: return "Game.NPC -> ";
		case LogChannel::GAME_PLAYER: return "Game.Player -> ";
		case LogChannel::GAME_SCREEN: return "Game.Screen -> ";

		case LogChannel::GAME_MAIN: return "Game.Main -> ";
	}
	return "Call func with error channel.";
}
auto GetCurrentTime() {
    const auto Time = std::chrono::system_clock::now();
	const auto TimeT = std::chrono::system_clock::to_time_t(Time);
    std::stringstream TimeString;
    TimeString << std::put_time(std::localtime(&TimeT), "%Y-%m-%d %X");
    return TimeString;
}

Log& Log::GetLogInstance() {
    static Log LogInstance;
    return LogInstance;
}

void Log::LogOut(const LogChannel channel, const LogLevel level, const std::string& logMessage) {
    std::lock_guard<std::mutex> lock(GetLogInstance().LogMutex);

	const std::string FullLogMessage = "[" + GetCurrentTime().str() + "] [" + GetLogLevel(level) + "] " + GetLogChannel(channel) + logMessage;
    std::cout << FullLogMessage << std::endl;
}

void Log::SetViewLogLevel(const LogLevel viewLogLevel) {
    GetLogInstance().ViewLogLevel = viewLogLevel;
}
