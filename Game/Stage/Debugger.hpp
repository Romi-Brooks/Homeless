// /**
//   * @file           : Debugger.hpp
//   * @author         : Romi Brooks
//   * @brief          :
//   * @attention      :
//   * @date           : 2025/9/23
//   Copyright (c) 2025 Romi Brooks, All rights reserved.
// **/
//
// #ifndef HOMELESS_DEBUGGER_HPP
// #define HOMELESS_DEBUGGER_HPP
//
// #include <imgui.h>
// #include <imgui-SFML.h>
//
// #include "../../Engine/Windows/ScreenManager.hpp"
// #include "../../Engine/Windows/Windows.hpp"
//
// using WindowsManager = engine::windows_manager::ScreenManager;
// class Debugger {
// 	private:
// 		engine::windows::RenderWindow& window_;
// 		sf::Clock deltaClock_;
//
// 		Debugger() : window_(WindowsManager::GetWindowsManager().CreateWindow("Debugger", 1920, 1080)) {
// 			window_.GetWindow()->setTitle("Debugger");
// 			// 缺少 ImGui-SFML 初始化
// 			if (!ImGui::SFML::Init(*window_.GetWindow())) {
// 				LOG_ERROR(LogChannel::ENGINE_WINDOWS_IMGUI, "Failed to initialize ImGui-SFML.");
// 			}
// 		}
//
// 		~Debugger() {
// 			ImGui::SFML::Shutdown();
// 		}
//
// 	public:
// 		static Debugger& GetDebugger() {
// 			static Debugger instance;
// 			return instance;
// 		}
//
// 		Debugger(const Debugger&) = delete;
// 		Debugger& operator=(const Debugger&) = delete;
//
// 	    auto Run() -> void {
// 	        while (window_.GetWindow()->isOpen()) {
// 	            // 事件处理
// 	            while (const auto event = window_.GetWindow()->pollEvent()) {
// 	                ImGui::SFML::ProcessEvent(*window_.GetWindow(), *event);
// 	                if (event->is<sf::Event::Closed>()) {
// 	                    window_.GetWindow()->close();
// 	                }
// 	            }
//
// 	            // 更新
// 	            ImGui::SFML::Update(*window_.GetWindow(), deltaClock_.restart());
//
// 	            // 设置背景颜色
// 	            constexpr sf::Color bgColor(255, 255, 255, 0);
// 	            window_.GetWindow()->clear(bgColor);
//
// 	            // 创建 ImGui 界面
// 	            CreateImGuiInterface();
//
// 	            // 渲染
// 	            ImGui::SFML::Render(*window_.GetWindow());
// 	            window_.GetWindow()->display();
// 	        }
// 	    }
//
// 	    auto CreateImGuiInterface() const -> void {
// 	        ImGui::Begin("Homeless ImGui Debugger");
//
// 	        ImGui::Text("Build Debugger with SFML3 and Imgui 1.92.2");
//
// 	        if (ImGui::Button("Load NPC_DeadBody")) {
// 	            // Generate::GenerateNPC(this->npc_, hl_npc::NPCName::NPC_DeadBody, *this->window_);
// 	        }
//
// 	        if (ImGui::Button("Load Player")) {
// 	            // Generate::GeneratePlayer(this->player_, "Romi", *this->window_);
// 	        }
//
// 	        if (ImGui::Button("Clear NPC&Player")) {
// 	            // this->ClearNPC();
// 	            // this->ClearPlayer();
// 	        }
//
// 	        if (ImGui::Button("Play Kick")) {
// 	            // engine::SFX::GetInstance().Play("Kick");
// 	        }
//
// 	        if (ImGui::Button("Play Snare")) {
// 	            // engine::SFX::GetInstance().Play("Snare");
// 	        }
//
// 	        if (ImGui::Button("Play Background")) {
// 	            // engine::Music::GetInstance().Play("bg1");
// 	        }
//
// 	        if (ImGui::Button("Switch Background")) {
// 	            // if (bool success = engine::sfx::MusicFade::GetInstance().Switch("bg1", "bg2", 3.0f)) {
// 	            //  LOG_INFO(LogChannel::ENGINE_AUDIO_MUSIC_FADE, "Started fade transition from bg1 to bg2");
// 	            // }
// 	        }
//
// 	        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
// 	                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//
// 	        ImGui::End();
// 	    }
//
// };
//
// #endif // HOMELESS_DEBUGGER_HPP
