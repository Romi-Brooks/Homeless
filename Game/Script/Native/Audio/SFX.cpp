/**
  * @file           : SFXLoader.cpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/11/25
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#include "../../../Config/Audios/SFX.hpp"
#include <Media/SFX.hpp>

auto LoadSFX() -> void {
	engine::audio::SFX::GetInstance().Load("native_kick", hl_sfx::native_kick_1);
	engine::audio::SFX::GetInstance().Load("native_snare", hl_sfx::native_snare_1);
}
