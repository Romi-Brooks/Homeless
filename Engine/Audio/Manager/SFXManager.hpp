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

#ifndef HOMELESS_SFXMANAGER_HPP
#define HOMELESS_SFXMANAGER_HPP

// Standard Libray
#include <fstream>
#include <string>
#include <unordered_map>
#include <memory>

// Third party Libray
#include <SFML/Audio/SoundBuffer.hpp>



namespace engine::audio {
	class SFXManager {
		private:
		    std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> soundBuffers_;

		    SFXManager() = default;

		public:
		    // Removing Copy Constructors and Assignment Operators
		    SFXManager(const SFXManager&) = delete;
		    SFXManager& operator=(const SFXManager&) = delete;

			// Get a singleton instance
			static SFXManager& GetManager();

		    // 加载SFX文件
		    bool LoadSFXFiles(const std::string& id, const std::string& filePath);

		    // 获取SFX缓冲区
		    sf::SoundBuffer* GetSFXBuffer(const std::string& id);

		    // 检查SFX是否已加载
		    bool HasSFX(const std::string& id) const;

		    // 卸载特定SFX
		    bool UnloadSFX(const std::string& id);

		    // 卸载所有SFX
		    void UnloadAll();

		    // 获取已加载SFX数量
		    size_t GetLoadedCount() const;
		};
}

#endif // HOMELESS_SFXMANAGER_HPP