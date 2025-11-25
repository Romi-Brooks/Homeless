/**
  * @file           : AudioLoader.cpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/11/25
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#include "Music.cpp"
#include "SFX.cpp"

namespace game::script::audio {
	auto LoadAudios() -> void {
		LoadMusic();
		LoadSFX();
	}
}