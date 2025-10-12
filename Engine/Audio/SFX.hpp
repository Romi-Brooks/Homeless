/**
  * @file           : SFX.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/14
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_SFX_HPP
#define HOMELESS_SFX_HPP

// Standard Libray
#include <memory>
#include <string>
#include <unordered_map>

// Third party Libray
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace engine::audio {
    class SFX final {
	    private:
	        // Sound instances mapped by ID
	        std::unordered_map<std::string, std::unique_ptr<sf::Sound>> sounds_;

	        // Global volume for all sounds
	        static float global_volume_;

	        // Private constructor for singleton
	        SFX() = default;

	    public:
	        // Delete copy constructor and assignment operator
	        SFX(const SFX&) = delete;
	        SFX& operator=(const SFX&) = delete;

	        // Get singleton instance
    		static SFX& GetInstance();

	        // Load a sound effect
	        auto Load(const std::string& id, const std::string& filePath) -> bool;

	        // Play a sound effect
	        auto Play(const std::string& id) -> void;

	        // Stop a specific sound effect
	        auto Stop(const std::string& id) -> void;
	        // Stop all sound effects
	        auto StopAll() -> void;

	        // Set volume for a specific sound effect
	        auto SetVolume(const std::string& id, float volume) -> void;

	        // Set global volume for all sound effects
	        static auto SetGlobalVolume(float volume) -> void;

	        // Get global volume
	        static auto GetGlobalVolume() -> float;

	        // Check if a sound effect is loaded
	        auto IsLoaded(const std::string& id) const -> bool;

	        auto GetSound(const std::string& id) -> sf::Sound*;

	        auto Reset() -> void;

	        // Update method to clean up finished sounds (if needed)
			static auto Update() -> void;
    };
}

#endif // HOMELESS_SFX_HPP