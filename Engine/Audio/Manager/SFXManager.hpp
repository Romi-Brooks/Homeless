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

// Standard Library
#include <string>
#include <unordered_map>
#include <memory>

// Third party Library
#include <SFML/Audio/SoundBuffer.hpp>

namespace engine::audio::manager {
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
		    auto LoadSFXFiles(const std::string& id, const std::string& filePath) -> bool;

		    // 获取SFX缓冲区
		    auto GetSFXBuffer(const std::string& id) -> sf::SoundBuffer*;

		    // 检查SFX是否已加载
		    auto HasSFX(const std::string& id) const -> bool;

		    // 卸载特定SFX
		    auto UnloadSFX(const std::string& id) -> bool;

		    // 卸载所有SFX
		    auto UnloadAll() -> void;

		    // 获取已加载SFX数量
		    auto GetLoadedCount() const -> size_t;
		};
}

#endif // HOMELESS_SFXMANAGER_HPP