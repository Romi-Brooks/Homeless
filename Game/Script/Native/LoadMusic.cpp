/**
  * @file           : LoadMusic.cpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/10/13
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#include "LoadMusic.hpp"
#include "../../Config/Audios/Music.hpp"
#include "../../Engine/Audio/Music.hpp"

namespace game::script::native {
	auto LoadMusic() -> void {
		engine::audio::Music::GetInstance().Load("bg1", hl_music::Background_1);
	}
}