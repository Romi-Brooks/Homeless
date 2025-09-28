/*  Copyright (c) 2025 Romi Brooks <qq1694821929@gmail.com>
 *  File Name: LogSystem.cpp
 *  Lib: Beeplayer I/O Log System
 *  Author: Romi Brooks
 *  Date: 2025-04-24
 *  Type: I/O, LOG System
 */

#include <string>
#include <iostream>
#include <mutex>
#include <chrono>
#include <sstream>
#include <iomanip>

#include "LogSystem.hpp"


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
	case LogChannel::ENGINE_AUDIO_MUSIC_FADE: return "Engine.Audio.Music.Fade -> ";
	case LogChannel::ENGINE_PHYSICS_EDGE_DETECT: return "Engine.Physics.EdgeDetect -> ";
	case LogChannel::ENGINE_MOVEMENT: return "Engine.Movement -> ";
	case LogChannel::ENGINE_WINDOW: return "Engine.Window -> ";
	case LogChannel::ENGINE_SCREEN: return  "Engine.Screen -> ";
	case LogChannel::ENGINE_SCREEN_MANAGER: return "Engine.Screen.Manager -> ";
	case LogChannel::GAME_MAIN: return "Game.Main -> ";
	}
	return "Call func with error channel.";
}
auto GetCurrentTime() {
    const auto Time = std::chrono::system_clock::now();
    auto TimeT = std::chrono::system_clock::to_time_t(Time);
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

void Log::SetViewLogLevel(LogLevel I_Level) {
    GetLogInstance().ViewLogLevel = I_Level;
}