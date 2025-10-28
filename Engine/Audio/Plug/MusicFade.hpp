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

		    FadeContext context_ {};
		    FadeCallback callback_ {};
		    std::thread fadeThread_ {};
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
		    auto Switch(const std::string& fromId, const std::string& toId, float duration = 2.0f) -> bool;

		    // 停止当前淡入淡出
		    auto Stop() -> void;

		    // 设置淡入淡出回调
		    auto SetCallback(const FadeCallback& callback) -> void;

		    // 获取当前状态
		    [[nodiscard]] auto GetState() const -> FadeState;

		    // 获取淡入淡出进度 (0.0 到 1.0)
		    [[nodiscard]] auto GetProgress() const -> float;

		    // 获取源音乐ID
		    [[nodiscard]] auto GetFromId() const -> std::string;

		    // 获取目标音乐ID
		    [[nodiscard]] auto GetToId() const -> std::string;

		    // 获取淡入淡出持续时间
		    [[nodiscard]] auto GetDuration() const -> float;

		    // 检查是否正在淡入淡出
		    [[nodiscard]] auto IsFading() const -> bool;

		private:
		    // 淡入淡出处理过程
		    auto FadeProcess() -> void;
		};
}

#endif // HOMELESS_MUSICFADE_HPP
