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

#include <Windows/Manager/ScreenManager.hpp>
#include <Windows/RenderWindow.hpp>
#include <Windows/Screen.hpp>

#include <Media/Manager/VolumeManager.hpp>
#include <Media/Music.hpp>
#include <Media/Plug/MusicFade.hpp>
#include <Media/SFX.hpp>

class Debugger : public engine::window::screen::Screen {
	private:
		// UI显示名 + 实际播放的资源名
		struct AudioResource {
			std::string display_name;  // UI显示的名称
			std::string resource_name; // 传给引擎的资源名
		};

		// 背景音乐列表
		const std::vector<AudioResource> background_musics = {
			{"Background 1", "native_background_1"},
			{"Background 2", "native_background_2"},
			{"Background 3", "lua_background_1"},
			{"Background 4", "lua_background_2"}
		};

		// SFX音效列表
		const std::vector<AudioResource> sfx_sounds = {
			{"Kick", "native_kick"},
			{"Snare", "native_snare"},
			{"Bird Call", "lua_niaojiao"}
		};

		void DrawAudioCombo() const {
			ImGui::Begin("Audio Player", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

			{
				// 背景音乐下拉选单
				static int selected_bg_index = 0; // 保存选中的索引
				ImGui::SeparatorText("Background Music");
				// 构建下拉框的显示名数组
				const char* current_bg = background_musics[selected_bg_index].display_name.c_str();
				if (ImGui::BeginCombo("Select Music", current_bg)) {
					for (int i = 0; i < background_musics.size(); i++) {
						const bool is_selected = (selected_bg_index == i);
						if (ImGui::Selectable(background_musics[i].display_name.c_str(), is_selected)) {
							selected_bg_index = i; // 更新选中索引
						}
						if (is_selected) {
							ImGui::SetItemDefaultFocus(); // 选中项自动聚焦
						}
					}
					ImGui::EndCombo();
				}

				// 播放选中的背景音乐
				ImGui::SameLine();
				if (ImGui::Button("Play Selected Music")) {
					const auto& [display_name, resource_name] = background_musics[selected_bg_index];
					engine::audio::Music::GetInstance().Play(resource_name);
				}
			}


			{
				// SFX下拉选单
				ImGui::SeparatorText("SFX Sounds");
				static int selected_sfx_index = 0;
				const char* current_sfx = sfx_sounds[selected_sfx_index].display_name.c_str();
				if (ImGui::BeginCombo("Select SFX", current_sfx)) {
					for (int i = 0; i < sfx_sounds.size(); i++) {
						const bool is_selected = (selected_sfx_index == i);
						if (ImGui::Selectable(sfx_sounds[i].display_name.c_str(), is_selected)) {
							selected_sfx_index = i; // 更新选中索引
						}
						if (is_selected) {
							ImGui::SetItemDefaultFocus(); // 选中项自动聚焦
						}
					}
					ImGui::EndCombo();
				}
				// 播放选中的SFX
				ImGui::SameLine();
				if (ImGui::Button("Play Selected SFX")) {
					const auto& [display_name, resource_name] = sfx_sounds[selected_sfx_index];
					engine::audio::SFX::GetInstance().Play(resource_name);
				}
			}

			{
			    ImGui::SeparatorText("Background Music (Fade Switch)");

			    // 获取当前播放的音乐
			    std::string current_playing = engine::audio::Music::GetInstance().GetNowPlaying();

			    // 显示当前播放的音乐
			    if (!current_playing.empty()) {
			        ImGui::Text("Now Playing: %s", current_playing.c_str());
			    } else {
			        ImGui::Text("Now Playing: None");
			    }

			    // 目标音乐下拉框
			    static int selected_to_bg_index = 0;
			    const char* current_to_bg = background_musics[selected_to_bg_index].display_name.c_str();

			    ImGui::Text("Switch To:");
			    ImGui::SameLine();
			    if (ImGui::BeginCombo("##to_music", current_to_bg, ImGuiComboFlags_NoArrowButton)) {
			        for (int i = 0; i < background_musics.size(); i++) {
			            const bool is_selected = (selected_to_bg_index == i);
			            if (ImGui::Selectable(background_musics[i].display_name.c_str(), is_selected)) {
			                selected_to_bg_index = i;
			            }
			            if (is_selected) {
			                ImGui::SetItemDefaultFocus();
			            }
			        }
			        ImGui::EndCombo();
			    }

			    ImGui::SameLine();

			    // 检查是否选择了当前正在播放的音乐
			    bool is_same_music = false;
			    if (!current_playing.empty()) {
			        const auto& [to_display, to_resource] = background_musics[selected_to_bg_index];
			        is_same_music = (current_playing == to_resource);
			    }

			    if (is_same_music) {
			        // 选中同一首音乐时，按钮置灰并显示警告
			        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
			        ImGui::Button("Switch");
			        ImGui::PopStyleColor();
			        // 红色警告文本
			        ImGui::SameLine();
			        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Same music selected!");
			    } else {
			        if (ImGui::Button("Switch")) {
			            const auto& [to_display, to_resource] = background_musics[selected_to_bg_index];
			            // 指定目标音乐
			            engine::audio::plug::MusicFade::GetInstance().Switch(to_resource, 2);
			        }
			    }

			    // 添加一个直接播放按钮
			    if (current_playing.empty()) {
			        ImGui::SameLine();
			        if (ImGui::Button("Play Directly")) {
			            const auto& [to_display, to_resource] = background_musics[selected_to_bg_index];
			            engine::audio::Music::GetInstance().Play(to_resource);
			        }
			    }
			}

			ImGui::End();
		}

		auto DrawFPSSelector() const -> void {
			if (ImGui::Button("Set FPS To 120")) {
				engine::window::RenderWindow::GetInstance().SetFPS(120);
			}

			if (ImGui::Button("Set FPS To 60")) {
				engine::window::RenderWindow::GetInstance().SetFPS(60);
			}
		}

		auto CreateImGuiInterface(sf::RenderWindow& window) const -> void {
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

			DrawAudioCombo();

			DrawFPSSelector();

			float currentVol = engine::audio::manager::VolumeManager::GetMusicVolume();
			if (ImGui::SliderFloat("Music Volume", &currentVol, 0.0f, 100.0f, "%.1f%%")	) {
				engine::audio::manager::VolumeManager::SetMusicVolume(currentVol);
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
