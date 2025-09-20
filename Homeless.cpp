#include "Stage/MainWindow.hpp"
#include "Engine/Audio/SFX.hpp"

#include "Engine/Config/Audios/SFX.hpp"
#include "Engine/Config/Audios/Music.hpp"

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

	MainWindow window(1920, 1080, "SFML + ImGui Test");
	window.Run();

	return 0;
}
