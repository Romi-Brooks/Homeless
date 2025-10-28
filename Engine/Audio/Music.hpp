/**
  * @file           : Music.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/20
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_MUSIC_HPP
#define HOMELESS_MUSIC_HPP

// Standard Library
#include <memory>
#include <string>
#include <unordered_map>
#include <chrono>

// Third party Library
#include <SFML/Audio/Music.hpp>

namespace engine::audio {
	// UI Background, Interactive Background...
	class Music {
		private:
			// Music instances mapped by ID
			std::unordered_map<std::string, std::unique_ptr<sf::Music>> musics_ {};

			// Global volume for all sounds
			static float music_volume_;

			// Private constructor for singleton
			Music() = default;

		public:
			// Delete copy constructor and assignment operator
			Music(const Music&) = delete;
			Music& operator=(const Music&) = delete;

			// Get singleton instance
			static Music& GetInstance();

			// Load a music file
			auto Load(const std::string& id, const std::string& file) -> bool;

			auto Play(const std::string& id) -> void;

			auto Stop(const std::string& id) -> void;

			// Set volume for a specific sound effect
			auto SetVolume(const std::string& id, float volume) -> void;

			// Instead, We play a sound effect with special volume
			auto Play(const std::string& id, float volume) -> void;

			// Volume Manager use this interface to set the playing time volume
			auto SetMusicVolume(float volume) const -> void;

			auto GetMusicVolume() const -> float;

			auto IsLoaded(const std::string& id) const -> bool;
	};
}


#endif //HOMELESS_MUSIC_HPP
