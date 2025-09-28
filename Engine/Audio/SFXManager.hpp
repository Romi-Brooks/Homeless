/**
  * @file           : SFXManager.hpp
  * @author         : Romi Brooks
  * @brief          : Sound Effect Resource Manager
  * @attention      :
  * @date           : 2025/9/19
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_SFXMANAGER_HPP
#define HOMELESS_SFXMANAGER_HPP

#include <fstream>
#include <string>
#include <unordered_map>
#include <memory>

#include <SFML/Audio/SoundBuffer.hpp>

#include "../Log/LogSystem.hpp"

namespace engine {
	class SFXManager {
		private:
		    std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> soundBuffers_;

		    // 私有构造函数确保单例模式
		    SFXManager() = default;

		public:
		    // 删除拷贝构造函数和赋值运算符
		    SFXManager(const SFXManager&) = delete;
		    SFXManager& operator=(const SFXManager&) = delete;

		    // 获取单例实例
		    static SFXManager& GetManager() {
		        static SFXManager manager;
		        return manager;
		    }

		    // 加载SFX文件
		    bool LoadSFXFiles(const std::string& id, const std::string& filePath) {
		        // 检查是否已加载
		        if (soundBuffers_.contains(id)) {
		        	LOG_WARNING(LogChannel::ENGINE_AUDIO_SFX, "SFX with id '" + id + "' is already loaded");
		            return true; // 已加载视为成功
		        }

		        // 1. 从文件加载
		        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
		        if (!file.is_open()) {
		        	LOG_ERROR(LogChannel::ENGINE_AUDIO_SFX, "Failed to open sfx file: " + filePath + " with id: " + id);
		            return false;
		        }

		        // 2. 获取文件大小
		        const std::streamsize size = file.tellg();
		        file.seekg(0, std::ios::beg);

		        // 3. 读取文件数据到vector
		        std::vector<char> data(size);
		        if (!file.read(data.data(), size)) {
		        	LOG_ERROR(LogChannel::ENGINE_AUDIO_SFX, "Failed to read sfx file: " + filePath + " with id: " + id);
		            file.close();
		            return false;
		        }

		        file.close(); // 文件读取完成

		        // 4. 创建SoundBuffer并从内存加载
		        auto buffer = std::make_unique<sf::SoundBuffer>();
		        if (!buffer->loadFromMemory(data.data(), data.size())) {
		        	LOG_ERROR(LogChannel::ENGINE_AUDIO_SFX, "Failed to load sfx data from memory for id: " + id);
		            return false;
		        }

		        // 5. 将buffer移动到map中（转移所有权）
		        soundBuffers_.emplace(id, std::move(buffer));
		    	LOG_INFO(LogChannel::ENGINE_AUDIO_SFX, "Successfully loaded SFX: " + id);

		        return true;
		    }

		    // 获取SFX缓冲区
		    sf::SoundBuffer* GetSFXBuffer(const std::string& id) {
		        auto it = soundBuffers_.find(id);
		        return (it != soundBuffers_.end()) ? it->second.get() : nullptr;
		    }

		    // 检查SFX是否已加载
		    bool HasSFX(const std::string& id) const {
		        return soundBuffers_.contains(id);
		    }

		    // 卸载特定SFX
		    bool UnloadSFX(const std::string& id) {
		        auto it = soundBuffers_.find(id);
		        if (it != soundBuffers_.end()) {
		            soundBuffers_.erase(it);
		        	LOG_INFO(LogChannel::ENGINE_AUDIO_SFX, "Unloaded SFX: " + id);
		            return true;
		        }
		    	LOG_WARNING(LogChannel::ENGINE_AUDIO_SFX, "SFX not found for unloading: " + id);
		        return false;
		    }

		    // 卸载所有SFX
		    void UnloadAll() {
		        soundBuffers_.clear();
		    	LOG_INFO(LogChannel::ENGINE_AUDIO_SFX, "All SFX resources unloaded");
		    }

		    // 获取已加载SFX数量
		    size_t GetLoadedCount() const {
		        return soundBuffers_.size();
		    }
		};

}

#endif // HOMELESS_SFXMANAGER_HPP