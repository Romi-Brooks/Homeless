/**
  * @file           : SFX.cpp
  * @author         : Romi Brooks
  * @brief          : 
  * @attention      : 
  * @date           : 2025/9/14
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

// Standard Libray
#include <ranges>

// Self Dependencies
#include "SFX.hpp"
#include "../Log/LogSystem.hpp"
#include "Manager/SFXManager.hpp"

namespace engine::audio {

	SFX& SFX::GetInstance() {
		static SFX instance;
		return instance;
	}

	auto SFX::Load(const std::string& id, const std::string& filePath) -> bool {
		// First, load the buffer through SFXManager
		if (!SFXManager::GetManager().LoadSFXFiles(id, filePath)) {
			LOG_ERROR(log::LogChannel::ENGINE_AUDIO_SFX, "Error when loading SFX: " + id);
			return false;
		}

		// Get the buffer from SFXManager
		const auto buffer = SFXManager::GetManager().GetSFXBuffer(id);
		if (!buffer) {
			LOG_ERROR(log::LogChannel::ENGINE_AUDIO_SFX, "Error getting buffer for SFX: " + id);
			return false;
		}

		// Create a new sound instance
		auto sound = std::make_unique<sf::Sound>(*buffer);

		// Store the sound instance
		sounds_[id] = std::move(sound);
		return true;
	}

	auto SFX::Play(const std::string& id) -> void {
		const auto it = sounds_.find(id);
		if (it != sounds_.end() && it->second) {
			it->second->play();
		} else {
			LOG_WARNING(log::LogChannel::ENGINE_AUDIO_SFX, "SFX not found or not loaded: " + id);
		}
	}

	auto SFX::Stop(const std::string& id) -> void {
		auto it = sounds_.find(id);
		if (it != sounds_.end() && it->second) {
			it->second->stop();
		}
	}

	auto SFX::StopAll() -> void {
		for (auto& it : sounds_ | std::views::values) {
			if (it) {
				it->stop();
			}
		}
	}

	auto SFX::SetVolume(const std::string& id, const float volume) -> void {
		const auto it = sounds_.find(id);
		if (it != sounds_.end() && it->second) {
			it->second->setVolume(volume);
		}
	}

	auto SFX::SetGlobalVolume(const float volume) -> void {
		global_volume_ = volume;
		auto& instance = GetInstance();
		for (auto& it : instance.sounds_ | std::views::values) {
			if (it) {
				it->setVolume(global_volume_);
			}
		}
	}

	auto SFX::GetGlobalVolume() -> float {
		return global_volume_;
	}

	auto SFX::IsLoaded(const std::string& id) const -> bool {
		const auto it = sounds_.find(id);
		return it != sounds_.end() && it->second != nullptr;
	}

	auto SFX::GetSound(const std::string& id) -> sf::Sound* {
		const auto it = sounds_.find(id);
		return (it != sounds_.end()) ? it->second.get() : nullptr;
	}

	auto SFX::Reset() -> void {
		StopAll();
		sounds_.clear();
	}

	auto SFX::Update() -> void {}
} // namespace engine::audio
