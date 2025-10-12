/**
  * @file           : SFXManager.hpp
  * @author         : Romi Brooks
  * @brief          : Sound Effect Resource Manager
  * @attention      : We use a singleton class manager to properly guide the "load", "unload" and "get" operations
  *					  of the SFX to ensure that the SFX is correctly and logically played by the engine.
  *					  playback by the engine in a logical and practical way.
  * @date           : 2025/9/19
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

// Self Dependencies
#include "SFXManager.hpp"
#include "../../Log/LogSystem.hpp"

namespace engine::audio {
	SFXManager& SFXManager::GetManager() {
		static SFXManager manager;
		return manager;
	}

	bool SFXManager::LoadSFXFiles(const std::string& id, const std::string& filePath) {
		// 检查是否已加载
		if (soundBuffers_.contains(id)) {
			LOG_WARNING(log::LogChannel::ENGINE_AUDIO_SFX, "SFX with id '" + id + "' is already loaded");
			return true; // 已加载视为成功
		}

		// 1. 从文件加载
		std::ifstream file(filePath, std::ios::binary | std::ios::ate);
		if (!file.is_open()) {
			LOG_ERROR(log::LogChannel::ENGINE_AUDIO_SFX, "Failed to open sfx file: " + filePath + " with id: " + id);
			return false;
		}

		// 2. 获取文件大小
		const std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		// 3. 读取文件数据到vector
		std::vector<char> data(size);
		if (!file.read(data.data(), size)) {
			LOG_ERROR(log::LogChannel::ENGINE_AUDIO_SFX, "Failed to read sfx file: " + filePath + " with id: " + id);
			file.close();
			return false;
		}

		file.close(); // 文件读取完成

		// 4. 创建SoundBuffer并从内存加载
		auto buffer = std::make_unique<sf::SoundBuffer>();
		if (!buffer->loadFromMemory(data.data(), data.size())) {
			LOG_ERROR(log::LogChannel::ENGINE_AUDIO_SFX, "Failed to load sfx data from memory for id: " + id);
			return false;
		}

		// 5. 将buffer移动到map中（转移所有权）
		soundBuffers_.emplace(id, std::move(buffer));
		LOG_INFO(log::LogChannel::ENGINE_AUDIO_SFX, "Successfully loaded SFX: " + id);

		return true;
	}

	sf::SoundBuffer* SFXManager::GetSFXBuffer(const std::string& id) {
		const auto it = soundBuffers_.find(id);
		return (it != soundBuffers_.end()) ? it->second.get() : nullptr;
	}

	bool SFXManager::HasSFX(const std::string& id) const  {
		return soundBuffers_.contains(id);
	}

	bool SFXManager::UnloadSFX(const std::string& id) {
		const auto it = soundBuffers_.find(id);
		if (it != soundBuffers_.end()) {
			soundBuffers_.erase(it);
			LOG_INFO(log::LogChannel::ENGINE_AUDIO_SFX, "Unloaded SFX: " + id);
			return true;
		}
		LOG_WARNING(log::LogChannel::ENGINE_AUDIO_SFX, "SFX not found for unloading: " + id);
		return false;
	}

	void SFXManager::UnloadAll() {
		soundBuffers_.clear();
		LOG_INFO(log::LogChannel::ENGINE_AUDIO_SFX, "All SFX resources unloaded");
	}

	size_t SFXManager::GetLoadedCount() const {
		return soundBuffers_.size();
	}
} // namespace engine::audio
