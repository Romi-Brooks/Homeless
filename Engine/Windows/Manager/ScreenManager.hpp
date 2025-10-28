/**
  * @file           : WindowsManager.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/23
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

// We use a "RenderWindow" class to make sure that all screen's (like 'StartScreen', 'HUD', 'Debugger', etc...)
// render are clearly and switchable, Render Window is singleton class that it has only one entity in all program,
// And it will render all things, if player selected some screen, use 'ScreenManager' class to switch it.

// to creat a screen, write a screen class with only render function, save to Screen Manager, use api to control it,
// Render Window will render it as well.

#ifndef HOMELESS_WINDOWSMANAGER_HPP
#define HOMELESS_WINDOWSMANAGER_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

#include "../Screen.hpp"



namespace engine::window::manager {
	class ScreenManager {
		private:
			// 存储所有已加载的屏幕
			std::unordered_map<std::string, std::unique_ptr<screen::Screen>> screens_ {};
			// 当前激活的屏幕 (栈顶屏幕)
			screen::Screen* currentScreen_ = nullptr;
			std::string currentScreenName_ {};
			// 改用vector模拟栈, 存储历史屏幕 (从底到顶)
			std::vector<std::pair<std::string, screen::Screen*>> screenStack_;


			ScreenManager() = default;
			~ScreenManager() = default;

		public:
			ScreenManager(const ScreenManager&) = delete;
			ScreenManager& operator=(const ScreenManager&) = delete;

			static ScreenManager& GetInstance();

			// 加载屏幕并注册到管理器
			auto LoadScreen(const std::string& name, std::unique_ptr<screen::Screen> screen) -> void;

			// 切换屏幕（替换当前屏幕，清空栈）
			auto SwitchScreen(const std::string& name) -> void;

			// 压入新屏幕（保留当前屏幕到栈中，新屏幕成为顶层）
			auto PushScreen(const std::string& name) -> void;

			// 弹出顶层屏幕（恢复栈顶的历史屏幕）
			auto PopScreen() -> void;

			// 多屏幕渲染：先渲染栈中所有历史屏幕（从底到顶），再渲染当前顶层屏幕
			auto Render(sf::RenderWindow& window) const -> void;

			// 事件处理：默认只让顶层屏幕处理事件（可根据需求修改为传递给所有屏幕）
			auto HandleEvent(const sf::Event& event) const -> void;

			// 更新逻辑：默认只更新顶层屏幕（可根据需求修改）
			auto Update(float deltaTime) const -> void;

			const std::string& GetCurrentScreenName() const;

			// 获取屏幕栈（返回const引用，避免复制）
			const std::vector<std::pair<std::string, screen::Screen*>>& GetScreenStack() const;
	};
}
#endif // HOMELESS_WINDOWSMANAGER_HPP
