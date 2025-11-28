/**
  * @file           : MusicLoader.cpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/11/25
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#include "../../../Config/Audios/Music.hpp"
#include <Media/Music.hpp>

auto LoadMusic() -> void {
	engine::audio::Music::GetInstance().Load("native_background_1", hl_music::native_background_1);
	engine::audio::Music::GetInstance().Load("native_background_2", hl_music::native_background_2);
}
