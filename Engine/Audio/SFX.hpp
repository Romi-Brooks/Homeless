/**
  * @file           : SFX.hpp
  * @author         : Romi Brooks
  * @brief          : Singleton SFX class for managing sound effects
  * @attention      : Simplified interface for playing sound effects
  * @date           : 2025/9/14
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_SFX_HPP
#define HOMELESS_SFX_HPP

#include <memory>
#include <ranges>
#include <string>
#include <unordered_map>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "SFXManager.hpp"
#include "../Log/LogSystem.hpp"

namespace engine {
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
	        static SFX& GetInstance() {
	            static SFX instance;
	            return instance;
	        }

	        // Load a sound effect
	        auto Load(const std::string& id, const std::string& filePath) -> bool {
	            // First, load the buffer through SFXManager
	            if (!SFXManager::GetManager().LoadSFXFiles(id, filePath)) {
	            	LOG_ERROR(LogChannel::ENGINE_AUDIO_SFX, "Error when loading SFX: " + id);
	                return false;
	            }

	            // Get the buffer from SFXManager
	            const auto buffer = SFXManager::GetManager().GetSFXBuffer(id);
	            if (!buffer) {
	            	LOG_ERROR(LogChannel::ENGINE_AUDIO_SFX, "Error getting buffer for SFX: " + id);
	                return false;
	            }

	            // Create a new sound instance
	            auto sound = std::make_unique<sf::Sound>(*buffer);

	            // Store the sound instance
	            sounds_[id] = std::move(sound);
	            return true;
	        }

	        // Play a sound effect
	        auto Play(const std::string& id) -> void{
	            const auto it = sounds_.find(id);
	            if (it != sounds_.end() && it->second) {
	                it->second->play();
	            } else {
	            	LOG_WARNING(LogChannel::ENGINE_AUDIO_SFX, "SFX not found or not loaded: " + id);
	            }
	        }

	        // Stop a specific sound effect
	        auto Stop(const std::string& id) -> void {
	            auto it = sounds_.find(id);
	            if (it != sounds_.end() && it->second) {
	                it->second->stop();
	            }
	        }

	        // Stop all sound effects
	        auto StopAll() -> void {
	            for (auto& it : sounds_ | std::views::values) {
	                if (it) {
	                    it->stop();
	                }
	            }
	        }

	        // Set volume for a specific sound effect
	        auto SetVolume(const std::string& id, const float volume) -> void {
	            const auto it = sounds_.find(id);
	            if (it != sounds_.end() && it->second) {
	                it->second->setVolume(volume);
	            }
	        }

	        // Set global volume for all sound effects
	        static auto SetGlobalVolume(const float volume) -> void{
	            global_volume_ = volume;
	            auto& instance = GetInstance();
	            for (auto& it : instance.sounds_ | std::views::values) {
	                if (it) {
	                    it->setVolume(global_volume_);
	                }
	            }
	        }

	        // Get global volume
	        static auto GetGlobalVolume() -> float {
	            return global_volume_;
	        }

	        // Check if a sound effect is loaded
	        auto IsLoaded(const std::string& id) const -> bool {
	            const auto it = sounds_.find(id);
	            return it != sounds_.end() && it->second != nullptr;
	        }

	        auto GetSound(const std::string& id) -> sf::Sound* {
	            const auto it = sounds_.find(id);
	            return (it != sounds_.end()) ? it->second.get() : nullptr;
	        }

	        auto Reset() -> void {
	            StopAll();
	            sounds_.clear();
	        }

	        // Update method to clean up finished sounds (if needed)
			static auto Update() -> void {
	        }
    };
}

#endif // HOMELESS_SFX_HPP