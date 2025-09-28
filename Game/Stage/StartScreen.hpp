/**
  * @file           : StartScreen.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/26
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_STARTSCREEN_HPP
#define HOMELESS_STARTSCREEN_HPP
#include <imgui.h>
#include <imgui-SFML.h>

#include "../../Engine/Windows/ScreenManager.hpp"
#include "../../Engine/Windows/RenderWindow.hpp"

class StartScreen : public engine::window::screen::Screen {
	private:

	public:
		void Render(sf::RenderWindow& window) override {
			ImGui::Begin("Start Screen");
			if (ImGui::Button("Start Game")) {
				engine::window::manager::ScreenManager::GetInstance().SwitchScreen("HUD");
			}

			if (ImGui::Button("Set FPS To 120")) {
				window.setFramerateLimit(120);
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::End();
		}

		void HandleEvent(const sf::Event& event) override {
		}

		void Update(float deltaTime) override {
		}
};


#endif // HOMELESS_STARTSCREEN_HPP
