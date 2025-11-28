/**
  * @file           : FadeSwitch.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/20
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_MUSICFADE_HPP
#define HOMELESS_MUSICFADE_HPP

// Standard Library
#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <thread>

namespace engine::audio::plug {
	enum class FadeState {
		Idle,
		FadingOut,
		FadingIn,
		Completed
	};

	class MusicFade {
		private:
			struct FadeContext {
				std::string fromId;
				std::string toId;
				float duration{0.0f};
				float progress{0.0f};
				FadeState state{FadeState::Idle};
			} context_;

			std::atomic<bool> stopRequested_{false};

			mutable std::mutex mutex_;

			std::thread fadeThread_;

			std::function<void(FadeState, const std::string&, const std::string&)> callback_;

			auto FadeProcess() -> void;

		public:
			static MusicFade& GetInstance() {
				static MusicFade instance;
				return instance;
			}

			~MusicFade();

			auto Switch(const std::string& toId, float duration) -> bool;
			auto Stop() -> void;

			auto SetCallback(const std::function<void(FadeState, const std::string&, const std::string&)>& callback) -> void;

			auto IsFading() const -> bool;

			auto GetState() const -> FadeState;
			auto GetProgress() const -> float;
			auto GetFromId() const -> std::string;
			auto GetToId() const -> std::string;
			auto GetDuration() const -> float;


		};
}

#endif // HOMELESS_MUSICFADE_HPP
