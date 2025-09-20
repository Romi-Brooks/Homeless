#ifndef HOMELESS_MUSIC_HPP
#define HOMELESS_MUSIC_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <chrono>

#include <SFML/Audio/Music.hpp>

#include "../Log/LogSystem.hpp"


namespace engine {
	// UI Background, Interactive Background...
	class Music {
		private:
			// Music instances mapped by ID
			std::unordered_map<std::string, std::unique_ptr<sf::Music>> musics_ {};

			// Global volume for all sounds
			static float global_volume_;

			// Private constructor for singleton
			Music() = default;

		public:
			// Delete copy constructor and assignment operator
			Music(const Music&) = delete;
			Music& operator=(const Music&) = delete;


			// Get singleton instance
			static Music& GetInstance() {
				static Music instance;
				return instance;
			}

			// Load a music file
			auto Load(const std::string& id, const std::string& file) -> bool {
				// 0. 检查是否已加载
				if (musics_.contains(id)) {
					Log::LogOut("Music with id '" + id + "' is already loaded", LogLevel::HL_WARNING);
					return true; // 已加载视为成功
				}

				// 1. 从文件加载
				auto music = std::make_unique<sf::Music>(file);

				if (!music) {
					Log::LogOut("Failed to load music from file for id: " + id, LogLevel::HL_ERROR);
					return false;
				}

				// 2. 存放到map
				musics_[id] = std::move(music);
				Log::LogOut("Successfully loaded music from file for id: " + id, LogLevel::HL_INFO);

				return true;
			}

			auto Play(const std::string& id, const float volume = 100.0f) -> void {
				auto it = musics_.find(id);
				if (it != musics_.end() && it->second) {
					it->second->setVolume(volume);
					it->second->play();
				}
			}

			auto Stop(const std::string& id) -> void {
				const auto it = musics_.find(id);
				if (it != musics_.end() && it->second) {
					it->second->stop();
				}
			}

			auto SetVolume(const std::string& id, const float volume) -> void {
				const auto it = musics_.find(id);
					if (it != musics_.end() && it->second) {
						it->second->setVolume(volume);
					}
				}

			auto IsLoaded(const std::string& id) const {
				if (!musics_.find(id)->second) {
					return false;
				}
				return true;
			}
	};
}


#endif //HOMELESS_MUSIC_HPP
