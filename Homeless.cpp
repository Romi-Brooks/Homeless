/**
  * @file           : Homeless.cpp
  * @author         : Romi Brooks
  * @brief          : This is the main program entry point for Homeless, where the program will begin execution.
  * @attention      : Prior to official release, all content here will be for testing purposes only—it is unsafe
					  and highly illogical.
  * @date           : 2025/10/12 (last edited)
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifdef _WIN32
#include "windows.h"
#endif // _WIN32

#include <thread>

#include "Engine/Audio/Music.hpp"
#include "Engine/Audio/SFX.hpp"

#include "Engine/Utilities/Random/Random.hpp"

#include "Engine/Windows/Manager/ScreenManager.hpp"
#include "Engine/Windows/RenderWindow.hpp"

#include "Game/Config/Audios/Music.hpp"
#include "Game/Config/Audios/SFX.hpp"

#include "Game/Stage/HUD.hpp"
#include "Game/Stage/Start.hpp"
#include "Lua/LuaLoader.hpp"


// Define static member
float engine::audio::SFX::global_volume_ = 100.0f;
float engine::audio::Music::global_volume_ = 100.0f;

auto main() -> int {
	#ifdef _WIN32
		SetConsoleOutputCP(CP_UTF8);
	#endif // _WIN32

	// RNG
	GlobalRandom::Init();

	// Lua
	LuaLoader luaLoader;
	if (!luaLoader.initialize()) {
		return -1;
	}
	const std::string MusicLoader = "Script/LoadMusic.lua";
	const std::string SFXLoader = "Script/LoadSFX.lua";
	luaLoader.loadScript(MusicLoader);
	luaLoader.loadScript(SFXLoader);

	try {
		// init render & window
		auto& renderWindow = engine::window::RenderWindow::GetInstance();
		renderWindow.Initialize("Homeless", 1920, 1080);

		// Register the screen
		auto& screenManager = engine::window::manager::ScreenManager::GetInstance();
		screenManager.LoadScreen("Start", std::make_unique<Start>());
		screenManager.LoadScreen("HUD", std::make_unique<HUD>());

		// default screen
		screenManager.SwitchScreen("Start");

		// run
		renderWindow.Run();

		return 0;
	} catch (const std::exception& e) {
		LOG_ERROR(engine::log::LogChannel::GAME_MAIN, "Application error.");
		return -1;
	}
}