/**
  * @file           : RenderWindow.hpp
  * @author         : Romi Brooks
  * @brief          : Main render window singleton (Engine Core)
  * @attention      :
  * @date           : 2025/9/28
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_RENDER_WINDOW_HPP
#define HOMELESS_RENDER_WINDOW_HPP

#include <SFML/Graphics.hpp>
#include "Manager/ScreenManager.hpp"

#include <imgui-SFML.h>

namespace engine::window {
	class RenderWindow {
		public:
			static RenderWindow& GetInstance() {
				static RenderWindow instance;
				return instance;
			}

			void Initialize(const std::string& title, unsigned width, unsigned height) {
				window_.create(sf::VideoMode({width, height}), title);
				window_.setFramerateLimit(fps_);

				// init ImGui-SFML
				if (!ImGui::SFML::Init(window_)) {
					throw std::runtime_error("Failed to initialize ImGui-SFML");
				}
			}

			void Run() {
				const auto& screenManager = manager::ScreenManager::GetInstance();

				// 检查是否有当前屏幕
				if (screenManager.GetCurrentScreenName().empty()) {
					throw std::runtime_error("No current screen set. Cannot run application.");
				}

				sf::Clock deltaClock;

				while (window_.isOpen()) {
					// Event Process
					ProcessEvents(screenManager);

					// Update
					ImGui::SFML::Update(window_, deltaClock.restart());

					// self & screen Render
					window_.clear();

					screenManager.Render(window_);

					ImGui::SFML::Render(window_);

					window_.display();
				}
				Shutdown();
			}

			auto SetFPS(const unsigned int fps) -> void {
				fps_ = fps;
				window_.setFramerateLimit(fps_);
			}

			sf::RenderWindow& GetWindow() {
				return window_;
			}

			const sf::RenderWindow& GetWindow() const {
				return window_;
			}

			auto GetFPS()const -> unsigned {
				return fps_;
			}

			bool IsOpen() const {
				return window_.isOpen();
			}

			void Shutdown() {
				ImGui::SFML::Shutdown();
				if (window_.isOpen()) {
					window_.close();
				}
			}
		private:
			sf::RenderWindow window_;
			unsigned int fps_ = 60;

			RenderWindow() = default;
			~RenderWindow() = default;

			void ProcessEvents(const manager::ScreenManager& screenManager) {
				while (const auto event = window_.pollEvent()) {
					ImGui::SFML::ProcessEvent(window_, *event);

					if (event->is<sf::Event::Closed>()) {
						window_.close();
					}
				}
			}


		};
}

#endif // HOMELESS_RENDER_WINDOW_HPP