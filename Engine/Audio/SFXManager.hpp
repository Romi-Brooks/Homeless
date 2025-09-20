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
		            Log::LogOut("SFX with id '" + id + "' is already loaded", LogLevel::HL_WARNING);
		            return true; // 已加载视为成功
		        }

		        // 1. 从文件加载
		        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
		        if (!file.is_open()) {
		            Log::LogOut("Failed to open sfx file: " + filePath + " with id: " + id, LogLevel::HL_ERROR);
		            return false;
		        }

		        // 2. 获取文件大小
		        const std::streamsize size = file.tellg();
		        file.seekg(0, std::ios::beg);

		        // 3. 读取文件数据到vector
		        std::vector<char> data(size);
		        if (!file.read(data.data(), size)) {
		            Log::LogOut("Failed to read sfx file: " + filePath + " with id: " + id, LogLevel::HL_ERROR);
		            file.close();
		            return false;
		        }

		        file.close(); // 文件读取完成

		        // 4. 创建SoundBuffer并从内存加载
		        auto buffer = std::make_unique<sf::SoundBuffer>();
		        if (!buffer->loadFromMemory(data.data(), data.size())) {
					Log::LogOut("Failed to load sfx data from memory for id: " + id, LogLevel::HL_ERROR);
		            return false;
		        }

		        // 5. 将buffer移动到map中（转移所有权）
		        soundBuffers_.emplace(id, std::move(buffer));
		        Log::LogOut("Successfully loaded SFX: " + id, LogLevel::HL_INFO);

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
		            Log::LogOut("Unloaded SFX: " + id, LogLevel::HL_INFO);
		            return true;
		        }
		        Log::LogOut("SFX not found for unloading: " + id, LogLevel::HL_WARNING);
		        return false;
		    }

		    // 卸载所有SFX
		    void UnloadAll() {
		        soundBuffers_.clear();
		        Log::LogOut("All SFX resources unloaded", LogLevel::HL_INFO);
		    }

		    // 获取已加载SFX数量
		    size_t GetLoadedCount() const {
		        return soundBuffers_.size();
		    }
		};

}

#endif // HOMELESS_SFXMANAGER_HPP