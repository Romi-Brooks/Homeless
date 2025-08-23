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


// Foward Function
std::string GetLogLevel(const LogLevel& LogLevel) {
    std::string LevelStr;
    switch (LogLevel) {
        case BP_INFO: LevelStr = "INFO"; break;
        case BP_WARNING: LevelStr = "WARNING"; break;
        case BP_ERROR: LevelStr = "ERROR"; break;
        case BP_DEBUG: LevelStr = "DEBUG"; break;
    }
    return LevelStr;
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

void Log::LogOut(const std::string& I_LogMessage, LogLevel I_Level) {
    std::lock_guard<std::mutex> lock(GetLogInstance().LogMutex);

    std::string FullLogMessage = "[" + GetCurrentTime().str() + "] [" + GetLogLevel(I_Level) + "] " + I_LogMessage;
    std::cout << FullLogMessage << std::endl;
}

void Log::SetViewLogLevel(LogLevel I_Level) {
    GetLogInstance().ViewLogLevel = I_Level;
}