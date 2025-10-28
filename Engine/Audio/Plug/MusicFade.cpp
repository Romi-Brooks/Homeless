/**
  * @file           : FadeSwitch.cpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/20
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

// Standard Library

#include <chrono>

// Self Dependencies
#include "../../Log/LogSystem.hpp"
#include "../Music.hpp"
#include "Audio/Manager/VolumeManager.hpp"
#include "MusicFade.hpp"

namespace engine::audio::plug {
	auto MusicFade::Switch(const std::string& fromId, const std::string& toId, const float duration) -> bool {
		// 检查是否已经在淡入淡出过程中
		if (context_.state != FadeState::Idle) {
			LOG_WARNING(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "Fade switch is already in progress");
			return false;
		}

		// 获取 Music 实例
		const auto& music = Music::GetInstance();

		// 检查音乐是否存在
		if (!music.IsLoaded(fromId) || !music.IsLoaded(toId)) {
			LOG_ERROR(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "Cannot switch music: one or both IDs not loaded");
			return false;
		}

		// 设置淡入淡出上下文
		context_.fromId = fromId;
		context_.toId = toId;
		context_.duration = duration;
		context_.progress = 0.0f;
		context_.state = FadeState::FadingOut;
		stopRequested_ = false;

		// 启动淡入淡出线程
		LOG_INFO(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "Switching music form " + fromId + " to " + toId);
		fadeThread_ = std::thread(&MusicFade::FadeProcess, this);

		// 通知回调
		if (callback_) {
			callback_(FadeState::FadingOut, fromId, toId);
		}

		return true;
	}

	auto MusicFade::Stop() -> void {
		if (context_.state != FadeState::Idle) {
			stopRequested_ = true;
			if (fadeThread_.joinable()) {
				fadeThread_.join();
			}
			context_.state = FadeState::Idle;
		}
	}

	auto MusicFade::SetCallback(const FadeCallback& callback) -> void {
		callback_ = callback;
	}

	auto MusicFade::GetState() const -> FadeState {
		return context_.state;
	}

	auto MusicFade::GetProgress() const -> float {
		return context_.progress;
	}

	auto MusicFade::GetFromId() const -> std::string {
		return context_.fromId;
	}

	auto MusicFade::GetToId() const -> std::string {
		return context_.toId;
	}

	auto MusicFade::GetDuration() const -> float {
		return context_.duration;
	}

	auto MusicFade::IsFading() const -> bool {
		return context_.state != FadeState::Idle;
	}

	auto MusicFade::FadeProcess() -> void {
	    auto& music = Music::GetInstance();
	    // 获取峰值音量
	    const float peakVolume = manager::VolumeManager::GetMusicVolume();
	    constexpr int steps = 100;
	    const int stepDuration = static_cast<int>((context_.duration * 1000) / (steps * 2));

	    // 淡出阶段：从峰值音量降到0
	    for (int i = 0; i <= steps; ++i) {
	        if (stopRequested_) {
	            context_.state = FadeState::Idle;
	            return;
	        }

	        // 音量计算
	        const float volume = peakVolume * (1.0f - static_cast<float>(i) / steps);
	        music.SetVolume(context_.fromId, volume);

	        context_.progress = static_cast<float>(i) / (steps * 2);
	        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(stepDuration)));
	    }

	    // 切换音乐
	    if (!stopRequested_) {
	        music.Stop(context_.fromId);
	        music.Play(context_.toId, 0.0f); // 从0音量开始
	        context_.state = FadeState::FadingIn;

	        if (callback_) {
	            callback_(FadeState::FadingIn, context_.fromId, context_.toId);
	        }
	    }

	    // 淡入阶段：从0升到峰值音量
	    for (int i = 0; i <= steps; ++i) {
	        if (stopRequested_) {
	            music.Stop(context_.toId);
	            context_.state = FadeState::Idle;
	            return;
	        }

	        // 音量计算
	        const float volume = peakVolume * (static_cast<float>(i) / steps);
	        music.SetVolume(context_.toId, volume);

	        context_.progress = 0.5f + (static_cast<float>(i) / (steps * 2));
	        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(stepDuration)));
	    }

	    // 完成
	    if (!stopRequested_) {
	        context_.state = FadeState::Completed;
	        if (callback_) {
	            callback_(FadeState::Completed, context_.fromId, context_.toId);
	        }

	    	LOG_INFO(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "Switching music completed");

	        // 短暂延迟后重置状态
	        std::this_thread::sleep_for(std::chrono::milliseconds(500l));
	        context_.state = FadeState::Idle;
	    }
}
}
