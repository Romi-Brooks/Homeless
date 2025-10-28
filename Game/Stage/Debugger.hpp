/**
  * @file           : Debugger.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/23
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_DEBUGGER_HPP
#define HOMELESS_DEBUGGER_HPP

#include <imgui.h>
#include <imgui-SFML.h>

#include "../../Engine/Windows/Manager/ScreenManager.hpp"
#include "../../Engine/Windows/Screen.hpp"
#include "Audio/Manager/VolumeManager.hpp"
#include "Audio/Plug/MusicFade.hpp"

using DebuggerWindowsManager = engine::window::manager::ScreenManager;

class Debugger : public engine::window::screen::Screen {
	private:
		static auto CreateImGuiInterface(sf::RenderWindow& window) -> void {
			ImGui::Begin("Homeless ImGui Debugger");

			ImGui::Text("Build Debugger with SFML3 and Imgui 1.92.2");

			if (ImGui::Button("Start Game")) {
				engine::window::manager::ScreenManager::GetInstance().SwitchScreen("Start");
				engine::window::manager::ScreenManager::GetInstance().PushScreen("Debugger");
			}

			if (ImGui::Button("Game Setting")) {
				engine::window::manager::ScreenManager::GetInstance().SwitchScreen("Setting");
				engine::window::manager::ScreenManager::GetInstance().PushScreen("Debugger");
			}

			if (ImGui::Button("Game HUD")) {
				engine::window::manager::ScreenManager::GetInstance().SwitchScreen("HUD");
				engine::window::manager::ScreenManager::GetInstance().PushScreen("Debugger");
			}

			if (ImGui::Button("Play Kick")) {
				engine::audio::SFX::GetInstance().Play("Kick");
			}

			if (ImGui::Button("Play Snare")) {
				engine::audio::SFX::GetInstance().Play("Snare");
			}

			if (ImGui::Button("Play Background")) {
				engine::audio::Music::GetInstance().Play("bg1");
			}

			if (ImGui::Button("Switch Background")) {
				engine::audio::plug::MusicFade::GetInstance().Switch("bg1", "bg2");
			}

			float currentVol = engine::audio::manager::VolumeManager::GetMusicVolume();
			if (ImGui::SliderFloat("Music Volume", &currentVol, 0.0f, 100.0f, "%.1f%%")	) {
				engine::audio::manager::VolumeManager::SetMusicVolume(currentVol);
			}

			if (ImGui::Button("Load Player")) {
				// Generate::GeneratePlayer(this->player_, "Romi", *this->window_);
			}

			if (ImGui::Button("Clear NPC&Player")) {
				// this->ClearNPC();
				// this->ClearPlayer();
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
						1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::End();
		}

	public:
		void Render(sf::RenderWindow& window) override {
			CreateImGuiInterface(window);
		}

		bool HandleEvent(const sf::Event& event) override {
			// // 假设Debugger有一个关闭按钮，点击时消费事件
			// if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			// 	const sf::Vector2f mousePos(
			// 		static_cast<float>(event.mouseButton.x),
			// 		static_cast<float>(event.mouseButton.y)
			// 	);
			// 	// 检查是否点击了Debugger的关闭按钮
			// 	if (closeButton.getGlobalBounds().contains(mousePos)) {
			// 		engine::window::manager::ScreenManager::GetInstance().PopScreen(); // 关闭自己
			// 		return true; // 消费事件，避免Start响应
			// 	}
			// }

			// 若点击的是Debugger的其他区域（如调试面板），也可根据需要消费
			// ...

			// 未消费事件，让下层处理
			return false;
		}

		void Update(float deltaTime) override {
		}

};

#endif // HOMELESS_DEBUGGER_HPP
