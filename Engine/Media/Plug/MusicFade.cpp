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
#include <mutex>
#include <atomic>

// Self Dependencies
#include "MusicFade.hpp"
#include "../Music.hpp"
#include "../Manager/VolumeManager.hpp"
#include "../../Log/LogSystem.hpp"

namespace engine::audio::plug {
	using FadeCallback = std::function<void(FadeState, const std::string&, const std::string&)>;

	MusicFade::~MusicFade() {
		Stop();
		// 确保线程正确结束
		if (fadeThread_.joinable()) {
			try {
				fadeThread_.join();
			}
			catch (...) {
				LOG_ERROR(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "Error when destructing the music entity.");
			}
		}
	}

	auto MusicFade::Switch(const std::string& toId, const float duration) -> bool {
	    std::lock_guard<std::mutex> lock(mutex_);
	    // 1. 停止正在进行的淡入淡出
	    if (context_.state != FadeState::Idle) {
	        stopRequested_ = true;
	        if (fadeThread_.joinable()) {
	            try {
	                fadeThread_.join();
	            }
	            catch (const std::exception& e) {
	                LOG_ERROR(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "Join old fade thread failed: " + std::string(e.what()));
	            }
	        }
	        stopRequested_ = false;
	        context_.state = FadeState::Idle;
	        fadeThread_ = std::thread{};
	    }

	    // 2. 获取当前播放的音乐作为源
	    auto& music = Music::GetInstance();
		const std::string fromId = music.GetNowPlaying();

	    // 3. 音乐是否已加载
	    if (!music.IsLoaded(toId)) {
	        LOG_ERROR(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "Cannot switch music: target music not loaded: " + toId);
	        return false;
	    }

	    // 4. 如果当前没有音乐在播放，直接开始播放目标音乐
	    if (fromId.empty()) {
	        LOG_INFO(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "No current music, starting playback of: " + toId);
	        music.Play(toId);
	        return true;
	    }

	    // 5. 如果源音乐和目标音乐相同，不需要切换
	    if (fromId == toId) {
	        LOG_WARNING(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "Source and target music are the same: " + fromId);
	        return true;
	    }

	    // 6. 目标音乐是否已加载
	    if (!music.IsLoaded(fromId)) {
	        LOG_ERROR(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "Cannot switch music: current playing music is not loaded: " + fromId);
	        return false;
	    }

	    // 7. 确保线程对象是干净的
	    if (fadeThread_.joinable()) {
	        fadeThread_.join();
	        fadeThread_ = std::thread{};
	    }

	    // 8. 初始化上下文
	    context_.fromId = fromId;
	    context_.toId = toId;
	    context_.duration = duration;
	    context_.progress = 0.0f;
	    context_.state = FadeState::FadingOut;

	    // 9. 启动新交换线程
	    try {
	        fadeThread_ = std::thread(&MusicFade::FadeProcess, this);
	        LOG_INFO(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "Switching music from " + fromId + " to " + toId);
	    }
	    catch (const std::exception& e) {
	        LOG_ERROR(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "Failed to start fade thread: " + std::string(e.what()));
	        context_.state = FadeState::Idle;
	        return false;
	    }

	    // 10. 回调通知
	    if (callback_) {
	        callback_(FadeState::FadingOut, fromId, toId);
	    }

	    return true;
	}

	// 停止
	auto MusicFade::Stop() -> void {
	    std::lock_guard<std::mutex> lock(mutex_);
	    if (context_.state == FadeState::Idle) {
	        return; // 无运行中的线程
	    }

	    // 触发停止请求
	    stopRequested_ = true;

	    // 等待线程结束
	    if (fadeThread_.joinable()) {
	        try {
	            fadeThread_.join();
	        } catch (const std::exception& e) {
	            LOG_ERROR(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "Join fade thread failed when stop: " + std::string(e.what()));
	        }
	    }

	    // 重置状态
	    context_.state = FadeState::Idle;
	    stopRequested_ = false;
		fadeThread_ = std::thread{};
	    LOG_INFO(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "Fade switch stopped");
	}

	// 设置回调
	auto MusicFade::SetCallback(const FadeCallback& callback) -> void {
	    std::lock_guard<std::mutex> lock(mutex_);
	    callback_ = callback;
	}

	// 获取状态
	auto MusicFade::GetState() const -> FadeState {
	    std::lock_guard<std::mutex> lock(mutex_);
	    return context_.state;
	}

	// 获取进度
	auto MusicFade::GetProgress() const -> float {
	    std::lock_guard<std::mutex> lock(mutex_);
	    return context_.progress;
	}

	// 获取源音乐ID
	auto MusicFade::GetFromId() const -> std::string {
	    std::lock_guard<std::mutex> lock(mutex_);
	    return context_.fromId;
	}

	// 获取目标音乐ID
	auto MusicFade::GetToId() const -> std::string {
	    std::lock_guard<std::mutex> lock(mutex_);
	    return context_.toId;
	}

	// 获取持续时间
	auto MusicFade::GetDuration() const -> float {
	    std::lock_guard<std::mutex> lock(mutex_);
	    return context_.duration;
	}

	// 判断是否正在淡入淡出
	auto MusicFade::IsFading() const -> bool {
	    std::lock_guard<std::mutex> lock(mutex_);
	    return context_.state != FadeState::Idle;
	}

	auto MusicFade::FadeProcess() -> void {
		// TODO: wtf, this function needs to be rewritten to use the proper throw an exception approach...
	    try {
	        auto& music = Music::GetInstance();
	        const float peakVolume = manager::VolumeManager::GetMusicVolume();
	        constexpr int steps = 100;
	        int stepDuration = 0;
	        std::string fromId, toId;

	        // 1.获取上下文信息
	        {
	            std::lock_guard<std::mutex> lock(mutex_);
	            stepDuration = static_cast<int>((context_.duration * 1000) / (steps * 2));
	            fromId = context_.fromId;
	            toId = context_.toId;
	        }

	        LOG_DEBUG(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "Starting fade process: " + fromId + " -> " + toId);

	        // 2.淡出阶段
	        for (int i = 0; i <= steps; ++i) {
	            if (stopRequested_) {
	                std::lock_guard<std::mutex> lock(mutex_);
	                context_.state = FadeState::Idle;
	                return;
	            }

	            const float volume = peakVolume * (1.0f - static_cast<float>(i) / steps);
	            music.SetVolume(fromId, volume);

	            {
	                std::lock_guard<std::mutex> lock(mutex_);
	                context_.progress = static_cast<float>(i) / (steps * 2);
	            }
	            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(stepDuration)));
	        }

	        // 3.切换音乐
	        if (!stopRequested_) {
	            music.Stop(fromId);
	            music.Play(toId, 0.0f);
	            music.SetNowPlaying(toId); // 确保当前播放状态更新

	            {
	                std::lock_guard<std::mutex> lock(mutex_);
	                context_.state = FadeState::FadingIn;
	                if (callback_) {
	                    callback_(FadeState::FadingIn, fromId, toId);
	                }
	            }
	        }

	        // 4.淡入阶段
	        for (int i = 0; i <= steps; ++i) {
	            if (stopRequested_) {
	                music.Stop(toId);
	                music.ClearNowPlaying();
	                std::lock_guard<std::mutex> lock(mutex_);
	                context_.state = FadeState::Idle;
	                return;
	            }

	            const float volume = peakVolume * (static_cast<float>(i) / steps);
	            music.SetVolume(toId, volume);

	            {
	                std::lock_guard<std::mutex> lock(mutex_);
	                context_.progress = 0.5f + (static_cast<float>(i) / (steps * 2));
	            }

	            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(stepDuration)));
	        }

	        // 5.完成阶段
	        if (!stopRequested_) {
	            std::lock_guard<std::mutex> lock(mutex_);
	            context_.state = FadeState::Completed;
	            if (callback_) {
	                callback_(FadeState::Completed, fromId, toId);
	            }
	            LOG_INFO(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "Switching music completed: " + fromId + " -> " + toId);

	            context_.state = FadeState::Idle;
	        }
	    }
		    catch (const std::exception& e) {
		        std::lock_guard<std::mutex> lock(mutex_);
		        LOG_ERROR(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "Exception in fade process: " + std::string(e.what()));
		        context_.state = FadeState::Idle;
		        Music::GetInstance().ClearNowPlaying();
		    }
		    catch (...) {
		        std::lock_guard<std::mutex> lock(mutex_);
		        LOG_ERROR(log::LogChannel::ENGINE_AUDIO_PLUG_MUSIC_FADE, "Unknown exception in fade process");
		        context_.state = FadeState::Idle;
		        Music::GetInstance().ClearNowPlaying();
		    }
	}
}