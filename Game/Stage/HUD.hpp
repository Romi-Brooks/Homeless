/**
  * @file           : HUD.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/26
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_HUD_HPP
#define HOMELESS_HUD_HPP
#include <imgui.h>
#include <imgui-SFML.h>

#include "../../Engine/Windows/Screen.hpp"
#include "../../Engine/Windows/RenderWindow.hpp"

class HUD : public engine::window::screen::Screen {
	public:
		void Render(sf::RenderWindow& window) override {
			ImGui::Begin("Game HUD");
			ImGui::Text("Health: 100");
			ImGui::Text("Ammo: 30");

			if (ImGui::Button("Back to Start")) {
				engine::window::manager::ScreenManager::GetInstance().SwitchScreen("Start");
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


#endif // HOMELESS_HUD_HPP
