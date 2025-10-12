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
#include <stack>
#include <memory>

#include "../Screen.hpp"

#include "../../Log/LogSystem.hpp"

namespace engine::window::manager {
	class ScreenManager {
		private:
			std::unordered_map<std::string, std::unique_ptr<screen::Screen>> screens_ {};
			screen::Screen* currentScreen_ = nullptr;
			std::string currentScreenName_ {};
			std::stack<std::pair<std::string, screen::Screen*>> screenStack_;


			ScreenManager() = default;
			~ScreenManager() = default;

		public:
			ScreenManager(const ScreenManager&) = delete;
			ScreenManager& operator=(const ScreenManager&) = delete;

			static ScreenManager& GetInstance() {
				static ScreenManager instance;
				return instance;
			}

			auto LoadScreen(const std::string& name, std::unique_ptr<screen::Screen> screen) -> void {
				screens_[name] = std::move(screen);
				LOG_INFO(log::LogChannel::ENGINE_SCREEN_MANAGER, "Registering screen: " + name);
			}

			auto SwitchScreen(const std::string& name) -> void { // switch, single render
				const auto it = screens_.find(name);
				if (it != screens_.end()) {
					if (currentScreen_) {
						currentScreen_->OnDeactivate();
					}
					currentScreen_ = it->second.get();
					currentScreenName_ = name;
					currentScreen_->OnActivate();
				}
			}

			auto PushScreen(const std::string& name) -> void { // cover render
				const auto it = screens_.find(name);
				if (it != screens_.end()) {
					if (currentScreen_) {
						screenStack_.push({currentScreenName_, currentScreen_});
						currentScreen_->OnDeactivate();
					}
					currentScreen_ = it->second.get();
					currentScreenName_ = name;
					currentScreen_->OnActivate();
				}
			}

			auto PopScreen() -> void { // the upper window
				if (!screenStack_.empty()) {
					if (currentScreen_) {
						currentScreen_->OnDeactivate();
					}
					const auto& previous = screenStack_.top();
					currentScreenName_ = previous.first;
					currentScreen_ = previous.second;
					screenStack_.pop();
					currentScreen_->OnActivate();
				}
			}

			auto Render(sf::RenderWindow& window) const -> void {
					if (currentScreen_) {
						currentScreen_->Render(window);
					}
				}

			auto HandleEvent(const sf::Event& event) const -> void {
					if (currentScreen_) {
						currentScreen_->HandleEvent(event);
					}
				}

			auto Update(const float deltaTime) const -> void {
					if (currentScreen_) {
						currentScreen_->Update(deltaTime);
					}
			}

			const std::string& GetCurrentScreenName() const {
				return currentScreenName_;
			}

			auto GetScreenStack() -> std::stack<std::pair<std::string, screen::Screen*>> {
				return screenStack_;
			}

	};
}
#endif // HOMELESS_WINDOWSMANAGER_HPP
