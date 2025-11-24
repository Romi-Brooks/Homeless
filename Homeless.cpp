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
#include <iostream>


#include "Engine/Audio/Music.hpp"
#include "Engine/Audio/SFX.hpp"

#include "Engine/Utilities/Random/Random.hpp"

#include "Engine/Components/Entities/Player.hpp"

#include "Engine/Tools/Packager/Packager.hpp"
#include "Engine/Tools/Packager/Unpackager.hpp"

#include "Engine/FileSystem/Encoder/Encoder.hpp"

#include "Engine/Windows/Manager/ScreenManager.hpp"
#include "Engine/Windows/RenderWindow.hpp"

#include "Engine/Lua/LuaLoader.hpp"

#include "Game/Script/Native/LoadMusic.hpp"

#include "Game/Stage/Debugger.hpp"
#include "Game/Stage/HUD.hpp"
#include "Game/Stage/Setting.hpp"
#include "Game/Stage/Start.hpp"

#include "Log/LogSystem.hpp"




auto main() -> int {
	#ifdef _WIN32
		SetConsoleOutputCP(CP_UTF8);
	#endif // _WIN32

	// RNG
	GlobalRandom::Init();

	// Resource loader
	game::script::native::LoadMusic();

	// Lua loader
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
		screenManager.LoadScreen("Debugger", std::make_unique<Debugger>());
		screenManager.LoadScreen("Start", std::make_unique<Start>());
		screenManager.LoadScreen("HUD", std::make_unique<HUD>());
		screenManager.LoadScreen("Setting", std::make_unique<Setting>());

		// default screen
		screenManager.SwitchScreen("Start");
		engine::window::manager::ScreenManager::GetInstance().PushScreen("Debugger");


		// run
		renderWindow.Run();

		return 0;
	} catch (const std::exception& e) {
		LOG_ERROR(engine::log::LogChannel::GAME_MAIN, "Application error: " + std::to_string(*e.what()));
		return -1;
	}
}