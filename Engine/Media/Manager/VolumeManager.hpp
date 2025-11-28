/**
  * @file           : VolumeManager.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/20
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_VOLUMEMANAGER_HPP
#define HOMELESS_VOLUMEMANAGER_HPP

namespace engine::audio::manager {
	class VolumeManager {
		private:
			VolumeManager() = default;

		public:

			static auto SetSfxVolume(float volume) -> void;

			static auto GetSfxVolume() -> float;

			static auto SetMusicVolume(float volume) -> void;

			static auto GetMusicVolume() -> float;

	};
}



#endif // HOMELESS_VOLUMEMANAGER_HPP
