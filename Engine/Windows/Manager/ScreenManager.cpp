/**
  * @file           : WindowsManager.cpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/23
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#include "ScreenManager.hpp"

#include <ranges>

#include "../../Log/LogSystem.hpp"

namespace engine::window::manager {
	ScreenManager& ScreenManager::GetInstance() {
		static ScreenManager instance;
		return instance;
	}

	auto ScreenManager::LoadScreen(const std::string& name, std::unique_ptr<screen::Screen> screen) -> void {
		screens_[name] = std::move(screen);
		LOG_INFO(log::LogChannel::ENGINE_SCREEN_MANAGER, "Registering screen: " + name);
	}

	auto ScreenManager::SwitchScreen(const std::string& name) -> void {
		const auto it = screens_.find(name);
		if (it != screens_.end()) {
			if (currentScreen_) {
				currentScreen_->OnDeactivate();
			}
			// 清空历史栈
			screenStack_.clear();
			// 更新当前屏幕
			currentScreen_ = it->second.get();
			currentScreenName_ = name;
			currentScreen_->OnActivate();
		}
		LOG_INFO(log::LogChannel::ENGINE_SCREEN_MANAGER, "Switched screen to : " + name);
	}

	auto ScreenManager::PushScreen(const std::string& name) -> void {
		const auto it = screens_.find(name);
		if (it != screens_.end()) {
			if (currentScreen_) {
				// 将当前屏幕压入栈（历史屏幕）
				screenStack_.emplace_back(currentScreenName_, currentScreen_);
				currentScreen_->OnDeactivate(); // 暂停当前屏幕
			}
			// 新屏幕成为当前顶层
			currentScreen_ = it->second.get();
			currentScreenName_ = name;
			currentScreen_->OnActivate();
		}
		LOG_INFO(log::LogChannel::ENGINE_SCREEN_MANAGER, "Pushed & Rendering screen: " + name);
	}

	auto ScreenManager::PopScreen() -> void  {
		if (!screenStack_.empty()) {
			if (currentScreen_) {
				currentScreen_->OnDeactivate();
			}
			// 恢复栈顶的历史屏幕
			const auto& previous = screenStack_.back();
			currentScreenName_ = previous.first;
			currentScreen_ = previous.second;
			screenStack_.pop_back(); // 移除栈顶
			currentScreen_->OnActivate(); // 恢复之前的屏幕
		}
		LOG_INFO(log::LogChannel::ENGINE_SCREEN_MANAGER, "Popped all screen: ");
	}

	auto ScreenManager::Render(sf::RenderWindow& window) const -> void {
		// 1. 渲染栈中的历史屏幕（下层屏幕）
		for (const auto& [name, screen] : screenStack_) {
			screen->Render(window);
		}
		// 2. 渲染当前顶层屏幕（覆盖在最上层）
		if (currentScreen_) {
			currentScreen_->Render(window);
		}
	}

	auto ScreenManager::HandleEvent(const sf::Event& event) const -> void {
		// 1. 优先让顶层屏幕处理（当前激活的屏幕，如Start）
		if (currentScreen_) {
			if (currentScreen_->HandleEvent(event)) {
				return; // 顶层屏幕消费了事件，不再传递给下层
			}
		}

		// 2. 若顶层未消费，再让栈中屏幕处理（如Debugger等下层屏幕）
		// 注意：栈中屏幕通常按“后入先出”顺序，可根据实际层级调整遍历方向
		for (const auto& screen : screenStack_ | std::views::values) {
			if (screen->HandleEvent(event)) {
				return; // 下层屏幕消费了事件，终止传递
			}
		}
	}

	auto ScreenManager::Update(const float deltaTime) const -> void {
		if (currentScreen_) {
			currentScreen_->Update(deltaTime);
		}
		// 如果需要让下层屏幕也更新，可添加：
		// for (const auto& [name, screen] : screenStack_) {
		//     screen->Update(deltaTime);
		// }
	}

	const std::string& ScreenManager::GetCurrentScreenName() const {
		return currentScreenName_;
	}

	const std::vector<std::pair<std::string, screen::Screen*>>& ScreenManager::GetScreenStack() const {
		return screenStack_;
	}

}