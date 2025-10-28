/**
  * @file           : Music.cpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/20
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

// Self Dependencies
#include "Music.hpp"
#include "../Log/LogSystem.hpp"

namespace engine::audio {

	Music& Music::GetInstance() {
		static Music instance;
		return instance;
	}

	auto Music::Load(const std::string& id, const std::string& file) -> bool {
		// 0. 检查是否已加载
		if (musics_.contains(id)) {
			LOG_WARNING(log::LogChannel::ENGINE_AUDIO_MUSIC, "Music with id '" + id + "' is already loaded");
			return true; // 已加载视为成功
		}

		// 1. 从文件加载
		auto music = std::make_unique<sf::Music>(file);

		if (!music) {
			LOG_ERROR(log::LogChannel::ENGINE_AUDIO_MUSIC, "Failed to load music from file for id: " + id);
			return false;
		}

		// 2. 存放到map
		musics_[id] = std::move(music);
		LOG_INFO(log::LogChannel::ENGINE_AUDIO_MUSIC, "Successfully loaded music from file for id: " + id);

		return true;
	}

	auto Music::Play(const std::string& id) -> void {
		const auto it = musics_.find(id);
		if (it != musics_.end() && it->second) {
			// we use Volume Manager class to make sure that audio plays well
			it->second->setVolume(music_volume_);

			it->second->play();
			LOG_INFO(log::LogChannel::ENGINE_AUDIO_MUSIC, "Playing: " + id);
		} else {
			LOG_WARNING(log::LogChannel::ENGINE_AUDIO_MUSIC, "Music not found or not loaded: " + id);
		}
	}

	auto Music::Stop(const std::string& id) -> void {
		const auto it = musics_.find(id);
		if (it != musics_.end() && it->second) {
			it->second->stop();
			LOG_INFO(log::LogChannel::ENGINE_AUDIO_MUSIC, "Stoping: " + id);
		}
	}

	auto Music::SetVolume(const std::string& id, const float volume) -> void {
		const auto it = musics_.find(id);
		if (it != musics_.end() && it->second) {
			it->second->setVolume(volume);
		}
	}

	auto Music::Play(const std::string& id, const float volume) -> void {
		const auto it = musics_.find(id);
		if (it != musics_.end() && it->second) {
			// use epically volume parameter
			it->second->setVolume(volume);
			it->second->play();
			LOG_INFO(log::LogChannel::ENGINE_AUDIO_MUSIC, "Playing: " + id + ", with volume: " + std::to_string(volume));
		} else {
			LOG_WARNING(log::LogChannel::ENGINE_AUDIO_MUSIC, "Music not found or not loaded: " + id);
		}
	}

	auto Music::SetMusicVolume(const float volume) const -> void {
		music_volume_ = volume;
		for (const auto& it : musics_) {
			it.second->setVolume(music_volume_);
		}
	}

	auto Music::GetMusicVolume() const -> float {
		return music_volume_;
	}

	auto Music::IsLoaded(const std::string& id) const -> bool {
		if (!musics_.find(id)->second) {
			return false;
		}
		return true;
	}


}
