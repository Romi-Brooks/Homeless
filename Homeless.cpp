#include <thread>

#include "Engine/Audio/Music.hpp"
#include "Engine/Audio/SFX.hpp"
#include "iostream"

#include "Engine/Utilities/Random/Random.hpp"

#include "Engine/Windows/RenderWindow.hpp"
#include "Engine/Windows/ScreenManager.hpp"

#include "Game/Config/Audios/Music.hpp"
#include "Game/Config/Audios/SFX.hpp"

#include "Game/Stage/HUD.hpp"
#include "Game/Stage/StartScreen.hpp"


// Define static member
float engine::SFX::global_volume_ = 100.0f;
float engine::Music::global_volume_ = 100.0f;

auto main() -> int {
	// RNG
	GlobalRandom::Init();

	// sfx load init
	engine::SFX::GetInstance().Load("Kick", hl_sfx::Kick);
	engine::SFX::GetInstance().Load("Snare", hl_sfx::Snare);

	// music load init
	engine::Music::GetInstance().Load("bg1", hl_music::youdiantian);
	engine::Music::GetInstance().Load("bg2", hl_music::fuyu);

	try {
		// 初始化引擎渲染窗口
		auto& renderWindow = engine::window::RenderWindow::GetInstance();
		renderWindow.Initialize("Homeless", 1920, 1080);

		// 注册游戏特定的屏幕
		auto& screenManager = engine::window::manager::ScreenManager::GetInstance();
		screenManager.LoadScreen("Start", std::make_unique<StartScreen>());
		screenManager.LoadScreen("HUD", std::make_unique<HUD>());

		// default screen
		screenManager.SwitchScreen("Start");

		// run
		renderWindow.Run();

		return 0;
	} catch (const std::exception& e) {
		LOG_ERROR(LogChannel::GAME_MAIN, "Application error.");
		return -1;
	}
}
