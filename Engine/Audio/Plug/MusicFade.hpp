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

// Standard Libray
#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <thread>

// Self Dependencies
#include "../../Log/LogSystem.hpp"
#include "../Music.hpp"


namespace engine::audio::plug {
	class MusicFade {
		public:
		    // 淡入淡出状态
		    enum class FadeState {
		        Idle,       // 空闲状态
		        FadingOut,  // 淡出中
		        FadingIn,   // 淡入中
		        Completed   // 完成
		    };

		    // 淡入淡出回调
		    using FadeCallback = std::function<void(FadeState state, const std::string& fromId, const std::string& toId)>;

		private:
		    // 淡入淡出状态
		    struct FadeContext {
		        std::atomic<FadeState> state{FadeState::Idle};
		        std::string fromId;
		        std::string toId;
		        float duration{2.0f};
		        float progress{0.0f}; // 0.0 到 1.0
		    };

		    FadeContext context_;
		    FadeCallback callback_;
		    std::thread fadeThread_;
		    std::atomic<bool> stopRequested_{false};

		    MusicFade() = default;

		public:
		    static MusicFade& GetInstance() {
		        static MusicFade instance;
		        return instance;
		    }

		    MusicFade(const MusicFade&) = delete;
		    MusicFade& operator=(const MusicFade&) = delete;

		    // 开始淡入淡出切换
		    auto Switch(const std::string& fromId, const std::string& toId,const float duration = 2.0f) -> bool {
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
		        fadeThread_ = std::thread(&MusicFade::FadeProcess, this);

		        // 通知回调
		        if (callback_) {
		            callback_(FadeState::FadingOut, fromId, toId);
		        }

		        return true;
		    }

		    // 停止当前淡入淡出
		    auto Stop() -> void {
		        if (context_.state != FadeState::Idle) {
		            stopRequested_ = true;
		            if (fadeThread_.joinable()) {
		                fadeThread_.join();
		            }
		            context_.state = FadeState::Idle;
		        }
		    }

		    // 设置淡入淡出回调
		    auto SetCallback(const FadeCallback& callback) -> void {
		        callback_ = callback;
		    }

		    // 获取当前状态
		    [[nodiscard]] auto GetState() const -> FadeState {
		        return context_.state;
		    }

		    // 获取淡入淡出进度 (0.0 到 1.0)
		    [[nodiscard]] auto GetProgress() const -> float {
		        return context_.progress;
		    }

		    // 获取源音乐ID
		    [[nodiscard]] auto GetFromId() const -> std::string {
		        return context_.fromId;
		    }

		    // 获取目标音乐ID
		    [[nodiscard]] auto GetToId() const -> std::string {
		        return context_.toId;
		    }

		    // 获取淡入淡出持续时间
		    [[nodiscard]] auto GetDuration() const -> float {
		        return context_.duration;
		    }

		    // 检查是否正在淡入淡出
		    [[nodiscard]] auto IsFading() const -> bool {
		        return context_.state != FadeState::Idle;
		    }

		private:
		    // 淡入淡出处理过程
		    auto FadeProcess() -> void {
		        auto& music = Music::GetInstance();
				constexpr int steps = 100;
		        const int stepDuration = static_cast<int>((context_.duration * 1000) / (steps * 2));

		        // 淡出阶段
		        for (int i = 0; i <= steps; ++i) {
		            if (stopRequested_) {
		                context_.state = FadeState::Idle;
		                return;
		            }

					const float volume = 100.0f - (static_cast<float>(i) * 100.0f / steps);
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

		        // 淡入阶段
		        for (int i = 0; i <= steps; ++i) {
		            if (stopRequested_) {
		                music.Stop(context_.toId);
		                context_.state = FadeState::Idle;
		                return;
		            }

					const float volume = static_cast<float>(i) * 100.0f / steps;
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

		            // 短暂延迟后重置状态
		            std::this_thread::sleep_for(std::chrono::milliseconds(500l));
		            context_.state = FadeState::Idle;
		        }
		    }
		};
}

#endif // HOMELESS_MUSICFADE_HPP
