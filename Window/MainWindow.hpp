#ifndef HOMELESS_MAINWINDOW_HPP
#define HOMELESS_MAINWINDOW_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "cstring"
#include "iostream"

#include "../Components/Entity.hpp"
#include "../Components/NPC.hpp"
#include "../Components/Player.hpp"
#include "../Engine/Generate.hpp"
#include "../Engine/Movement.hpp"

class MainWindow {
	private:
		// sfml basic controller
		sf::RenderWindow *window_ = nullptr;
		sf::Texture *texture_ = nullptr;
		sf::Sprite *sprite_ = nullptr;
		sf::Font *font_ = nullptr;
		sf::Text *text_ = nullptr;
		sf::Music *music_ = nullptr;

		sf::VertexArray connections_;

		sf::Clock deltaClock_;

		std::vector<NPC> npc_;
		std::vector<Player> player_;
		size_t selectedNPCIndex = -1;  // -1表示未选中
		size_t selectedPlayerIndex = -1;  // -1表示未选中

		// DPI zoom
		float dpiScale_ = 1.0f;
		unsigned int maxFrames_ = 120;
		bool isMusicLoad_ = false;
		bool isFullscreen_ = false;
		// bool showDemoWindow_ = true; // 控制是否显示ImGui演示窗口

		ImVec4 clearColor_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // 背景颜色

		// ImGui Text buffer
		char textInputBuffer[256] = ""; // 可以根据需要调整缓冲区大小
		bool textInputActive = false;   // 用于跟踪文本框是否处于活动状态

		auto DetectAndSetDPIScaling() -> void {
			// 获取主显示器的DPI
			sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

			// 简单估算DPI缩放因子
			// 标准DPI是96，如果屏幕宽度大于1920，则增加缩放
			if (desktopMode.size.x > 2560) {
				dpiScale_ = 2.0f; // 4K屏幕
			} else if (desktopMode.size.x > 1920) {
				dpiScale_ = 1.5f; // 2K屏幕
			} else {
				dpiScale_ = 1.0f; // 全高清
			}

			// 应用缩放到ImGui
			ImGui::GetStyle().ScaleAllSizes(dpiScale_);
			ImGui::GetIO().FontGlobalScale = dpiScale_;

			// 重新创建字体纹理以适应新的缩放
			if (ImGui::SFML::UpdateFontTexture()) {
				return;
			}

		}

	public:
		MainWindow(const unsigned int x, const unsigned int y, const std::string& Tittle) {
			this->InitWindow(x, y, Tittle);
			this->window_->setFramerateLimit(this->maxFrames_);
			this->InitFontTexture();
			this->InitSprite(x, y);

			this->InitImGui(); // Imgui

			// this->DetectAndSetDPIScaling();		// there are some bugs, so don't use it before fixed it.

			this->npc_ = std::vector<NPC>();
			this->player_ = std::vector<Player>();

			this->connections_.setPrimitiveType(sf::PrimitiveType::Lines);

		}

		MainWindow(const unsigned int x, const unsigned int y, const std::string& Tittle, unsigned int Frames)
		: MainWindow(x, y, Tittle) {
			this->maxFrames_ = Frames;
			this->window_->setFramerateLimit(this->maxFrames_);
		}

		auto InitWindow(const unsigned int x, const unsigned int y, const std::string& Tittle) -> bool {
			this->window_ = new sf::RenderWindow(sf::VideoMode({x, y}), Tittle);
			return true;
		}

		auto InitFontTexture()-> bool {
			this->font_ = new sf::Font("Media/arial.ttf");
			return true;
		}

		auto InitImGui() const -> bool {
			// 初始化ImGui-SFML
			if (!ImGui::SFML::Init(*window_)) {
				return false;
			}

			// 设置ImGui样式
			ImGui::StyleColorsDark();

			// 获取ImGui样式引用
			ImGuiStyle& style = ImGui::GetStyle();

			// 根据DPI缩放调整样式
			style.ScaleAllSizes(dpiScale_);

			// 加载字体 - 使用更大的基础字体大小
			ImGuiIO& io = ImGui::GetIO();

			// 或者加载自定义字体大小
			ImFont* font = io.Fonts->AddFontFromFileTTF("Media/arial.ttf", 16.0f * dpiScale_);
			io.FontDefault = font;

			// 创建字体纹理
			if (!ImGui::SFML::UpdateFontTexture()) {
				return false;
			}

			return true;

		}

		auto InitSprite(const unsigned int x, const unsigned int y) -> bool {
			// first init texture
			this->texture_ = new sf::Texture(sf::Vector2u(x,y));

			// Set Sprite from texture
			this->sprite_ = new sf::Sprite(*this->texture_);

			return true;
		}

		auto LoadTexture(const std::string& filename) -> bool {
			if (this->texture_->loadFromFile(filename)) {
				this->sprite_ = new sf::Sprite(*this->texture_);
				return true;
			}
			return false;
		}

		auto LoadText(const std::string& text) -> bool {
			// 确保字体已加载
			if (this->font_ == nullptr) {
				std::cerr << "Font not loaded! Cannot create text." << std::endl;
				return false;
			}

			// 如果已有文本对象，先删除
			if (this->text_ != nullptr) {
				delete this->text_;
				this->text_ = nullptr;
			}

			// 创建新的文本对象
			this->text_ = new sf::Text(*this->font_, text, static_cast<unsigned int>(50 * dpiScale_));

			// 设置文本位置（居中显示）
			this->text_->setPosition({this->window_->getSize().x / 2.0f,
									this->window_->getSize().y / 2.0f});

			// 设置文本颜色
			this->text_->setFillColor(sf::Color::White);

			// 添加轮廓（可选）
			this->text_->setOutlineColor(sf::Color::Black);
			this->text_->setOutlineThickness(2.0f);

			std::cout << "Text created: " << text << std::endl;
			return true;
		}

		auto LoadMusic() -> bool {
			if (this->isMusicLoad_) {
				return false;
			}

			this->music_ = new sf::Music("Media/汪苏泷,By2 - 有点甜.mp3");
			this->isMusicLoad_ = true;
			return true;
		}

		auto PlayMusic() const -> bool {
			if (!this->music_) {
				return false;
			}
			this->music_->play();
			return true;
		}

		auto SetToFullscreen() -> void {
				// 切换到全屏模式
				this->InitSprite(sf::VideoMode::getFullscreenModes()[0].size.x,sf::VideoMode::getFullscreenModes()[0].size.y);

				this->window_->create(sf::VideoMode::getFullscreenModes()[0], "SFML + ImGui Test", sf::State::Fullscreen);
				sf::sleep(sf::milliseconds(100));
				this->window_->setFramerateLimit(this->maxFrames_);

				this->UpdateAfterResolutionChange();

				isFullscreen_ = true;
				std::cout << "Set to Fullscreen mode." << std::endl;
			}

		auto SetToWindowed() -> void {
				this->InitSprite(this->window_->getSize().x, this->window_->getSize().y);

				this->window_->create(sf::VideoMode({1920, 1080}), "SFML Window", sf::Style::Default);
				sf::sleep(sf::milliseconds(100));
				this->window_->setFramerateLimit(this->maxFrames_);

				this->UpdateAfterResolutionChange();

				isFullscreen_ = false;
				std::cout << "Set to Windowed mode." << std::endl;
			}

		auto UpdateAfterResolutionChange() const -> void {
				sf::View view = this->window_->getView();
				view.setSize({this->window_->getSize().x / dpiScale_, this->window_->getSize().y / dpiScale_});
				this->window_->setView(view);

				if (this->text_ != nullptr) {
					this->text_->setPosition({this->window_->getSize().x / 2.0f,
											this->window_->getSize().y / 2.0f});
				}
			}

		auto EntityRender() const -> void {
			for (auto& npc : this->npc_) {
				auto* shape = npc.GetShape();
				if (npc.IsSelected()) {
					shape->setOutlineColor(sf::Color::Yellow);
					shape->setOutlineThickness(3.f);
					Move(npc, *this->window_);
				} else {
					shape->setOutlineThickness(0.f);
				}
				npc.Draw(*this->window_);
			}

			for (auto& player : this->player_) {
				auto* shape = player.GetShape();
				if (player.IsSelected()) {
					shape->setOutlineColor(sf::Color::Yellow);
					shape->setOutlineThickness(3.f);
					Move(player, *this->window_);
				} else {
					shape->setOutlineThickness(0.f);
				}
				player.Draw(*this->window_);
			}
		}

		auto RightMouseClick() -> void {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
				sf::Vector2i mousePosWindow = sf::Mouse::getPosition(*this->window_);
				sf::Vector2f mousePosWorld = this->window_->mapPixelToCoords(mousePosWindow);

				// reset selected status
				selectedNPCIndex = -1;
				selectedPlayerIndex = -1;
				for (auto& npc : this->npc_) npc.SetSelected(false);
				for (auto& player : this->player_) player.SetSelected(false);

				// index check
				for (size_t i = 0; i < this->npc_.size(); ++i){
					auto& npc = this->npc_[i];
					if (npc.GetShape()->getGlobalBounds().contains(mousePosWorld)) {
						npc.SetSelected(true);
						selectedNPCIndex = i;
						break;
					}
				}

				for (size_t i = 0; i < player_.size(); ++i) {
					auto& player = this->player_[i];
					if (player.GetShape()->getGlobalBounds().contains(mousePosWorld)) {
						player.SetSelected(true);
						selectedPlayerIndex = i;
						selectedNPCIndex = -1;
						break;
					}
				}
			}

		}

		// 检测范围内NPC并创建连接线
		void UpdateNPCConnections() {
			connections_.clear();  // 清除上一帧的连线

			// 检测并添加有效连线
			for (const auto& npc : npc_) {
				if (player_.begin()->IsNPCInRange(npc)) {
					// 添加线段的两个端点（玩家→NPC）
					sf::Vector2f pPos = player_.begin()->GetPosition();
					sf::Vector2f nPos = npc.GetPosition();

					connections_.append(sf::Vertex(pPos, sf::Color::Yellow));
					connections_.append(sf::Vertex(nPos, sf::Color::Yellow));
				}
			}
		}

		auto ClearNPC() -> void {
			this->npc_.clear();
		}
		auto ClearPlayer() -> void {
				this->player_.clear();
		}

		~MainWindow() {
			ImGui::SFML::Shutdown(); // 关闭ImGui
			delete window_;
			delete texture_;
			delete sprite_;
			delete font_;
			delete music_;
		}

		auto Run() -> void {
		    while (this->window_->isOpen()) {
		        // 事件处理
		        while (const auto event = this->window_->pollEvent()) {
		            ImGui::SFML::ProcessEvent(*window_, *event);
		            if (event->is<sf::Event::Closed>()) {
		                this->window_->close();
		            }
		        }

		        // 更新ImGui
		        ImGui::SFML::Update(*this->window_, this->deltaClock_.restart());
		        this->CreateImGuiInterface();

		        // 清除屏幕
		        this->window_->clear();

		    	// right mouse click
		    	this->RightMouseClick();


		    	if (!this->player_.empty()) {
		    		auto& player = *this->player_.begin(); // 获取玩家实例
		    		sf::Vector2f playerPos = player.GetPosition(); // 玩家当前位置

		    		// 4.1 同步玩家判定圈位置
		    		player.UpdateDetectionCircle();

		    		// 4.2 清空上一帧的连接线（避免重复绘制）
		    		connections_.clear();

		    		// 4.3 遍历所有NPC（假设你有NPC容器npcs_，如std::vector<NPC> npcs_）
		    		for (auto& npc : npc_) {
		    			// 4.3.1 判断NPC是否在玩家判定圈内（用Player类自带的精确检测）
		    			bool isInDetectionRange = player.IsNPCInRange(npc);

		    			// 4.3.2 同步NPC的碰撞状态（用于NPC朝向玩家移动）
		    			npc.SetPlayerCollisionStatus(isInDetectionRange);

		    			// 4.3.3 更新NPC移动（传入玩家位置，触发追踪逻辑）
		    			npc.UpdateMovement(playerPos);

		    			// 4.3.4 若在范围内：添加“玩家→NPC”的连线
		    			if (isInDetectionRange) {
		    				sf::Vector2f npcPos = npc.GetPosition(); // NPC当前位置

		    				// 连接线颜色（半透明白色，可自定义）
		    				sf::Color lineColor(255, 255, 255, 150);

		    				// 添加连线的两个顶点：玩家位置 → NPC位置
		    				connections_.append(sf::Vertex(playerPos, lineColor));
		    				connections_.append(sf::Vertex(npcPos, lineColor));
		    			}
		    		}
		    	}


				// npc & player render
				this->EntityRender();

				// ImGuiItemSelectedPopUp Render
				this->ImGuiItemSelectedPopUp();

		        {
		        	if (!this->player_.empty()) {
		        		// 更新玩家判定圈位置
		        		player_.begin()->UpdateDetectionCircle();

		        		// 检测范围内的NPC并更新连接线
		        		UpdateNPCConnections();

		        		// 绘制连接线（最后绘制，确保显示在最上层）
		        		this->window_->draw(connections_);
		        	}
		        }

		        // 绘制其他元素
		        this->window_->draw(*this->sprite_);
		        if (this->text_ != nullptr) {
		            this->window_->draw(*this->text_);
		        }

		        // 渲染ImGui（必须在所有绘制之后）
		        ImGui::SFML::Render(*this->window_);
		        this->window_->display();
		    }
		}


		auto CreateImGuiInterface() -> void {
				// 1. 显示一个简单的窗口
				{
					ImGui::Begin("Homeless ImGui Debugger"); // 创建一个窗口

					// 添加文本
					ImGui::Text("Build Debugger with SFML3 and Imgui 1.92.2");

					// 动态切换按钮
					if (isFullscreen_) {
						// 当前是全屏模式，显示"切换到窗口模式"按钮
						if (ImGui::Button("Windowed")) {
							this->SetToWindowed();
						}
					} else {
						// 当前是窗口模式，显示"切换到全屏模式"按钮
						if (ImGui::Button("Fullscreen")) {
							this->SetToFullscreen();
						}
					}


					if (ImGui::Button("Load NPC_DeadBody")) {
						Generate::GenerateNPC(this->npc_, hl_npc::NPCName::NPC_Name_DeadBody, *this->window_);
						// reset selected status
						selectedNPCIndex = -1;
						selectedPlayerIndex = -1;
					}

					if (ImGui::Button("Load Player")) {
						Generate::GeneratePlayer(this->player_, "Romi", *this->window_);
						// reset selected status
						selectedNPCIndex = -1;
						selectedPlayerIndex = -1;
					}

					if (ImGui::Button("Clear NPC&Player")) {
						this->ClearNPC();
						this->ClearPlayer();
						selectedNPCIndex = -1;
						selectedPlayerIndex = -1;
					}

					if (ImGui::Button("Load Background")) {
						this->LoadTexture("Media/1.jpg");
					}

					if (ImGui::Button("Unload Background")) {
						this->InitSprite(this->window_->getSize().x, this->window_->getSize().y);
					}

					if (ImGui::Button("Load Music")) {
						this->LoadMusic();
					}

					if (ImGui::Button("Play Music")) {
						this->PlayMusic();
					}

					// Load Text
					{
						ImGui::Begin("Text Loader");

						ImGui::Text("Text:");

						if (ImGui::InputText("##Input Label", textInputBuffer, IM_ARRAYSIZE(textInputBuffer),
											ImGuiInputTextFlags_EnterReturnsTrue))
						{
							// 当用户按下回车键时调用LoadText
							if (std::strlen(textInputBuffer) > 0) {
								this->LoadText(std::string(textInputBuffer));
							}
						}

						// 跟踪文本框是否处于活动状态
						textInputActive = ImGui::IsItemActive();

						// 添加一个按钮来提交文本
						if (ImGui::Button("Submit")) {
							if (std::strlen(textInputBuffer) > 0) {
								this->LoadText(std::string(textInputBuffer));
							}
						}

						// 添加一个清空按钮
						ImGui::SameLine();
						if (ImGui::Button("Clear")) {
							std::memset(textInputBuffer, 0, sizeof(textInputBuffer));
						}

						// 显示当前文本长度
						ImGui::Text("Length: %zu ch", std::strlen(textInputBuffer));

						ImGui::End();
					}

					// 添加帧率显示
					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
								1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

					ImGui::End();
				}

				// 设置窗口背景颜色
				sf::Color bgColor(
					static_cast<int8_t>(clearColor_.x * 255),
					static_cast<int8_t>(clearColor_.y * 255),
					static_cast<int8_t>(clearColor_.z * 255),
					static_cast<int8_t>(clearColor_.w * 255)
				);

				this->window_->clear(bgColor);
			}

			auto ImGuiItemSelectedPopUp() const -> void {
				if (selectedNPCIndex != -1 && selectedNPCIndex < static_cast<int>(npc_.size())) {
					auto& selectedNPC = npc_[selectedNPCIndex];
					ImGui::Begin("NPC Status");

					ImGui::Text("Active NPC");
					ImGui::Text("Name: %s", hl_trans::GetNPCName(selectedNPC.GetNPCName()).c_str());
					ImGui::Text("Type: %s", hl_trans::GetNPCType(selectedNPC.GetNPCType()).c_str());
					ImGui::Text("Killable: %s", hl_trans::GetNPCKillable(selectedNPC.GetNPCKillable()).c_str());
					ImGui::Text("HP: %d", selectedNPC.GetHP());
					ImGui::Text("Attack: %d", selectedNPC.GetAttack());
					ImGui::Text("Position: %.1f, %.1f",
							   selectedNPC.GetPosition().x,
							   selectedNPC.GetPosition().y);

					ImGui::End();
				}


				if (selectedPlayerIndex != -1 && selectedPlayerIndex < static_cast<int>(player_.size())) {
					auto& selectedPlayer = player_[selectedPlayerIndex];
					ImGui::Begin("Player Status");

					ImGui::Text("Active Player");
					ImGui::Text("Name: %s", selectedPlayer.GetPlayerName().c_str());
					ImGui::Text("Money: %d", selectedPlayer.GetPlayerMoney());
					ImGui::Text("Exp: %d", selectedPlayer.GetPlayerExp());
					ImGui::Text("HP: %d", selectedPlayer.GetHP());
					ImGui::Text("Attack: %d", selectedPlayer.GetAttack());
					ImGui::Text("Position: %.1f, %.1f",
							   selectedPlayer.GetPosition().x,
							   selectedPlayer.GetPosition().y);

					ImGui::End();
				}
			}
		MainWindow(const MainWindow&) = delete;
		MainWindow& operator=(const MainWindow&) = delete;
};

#endif //HOMELESS_MAINWINDOW_HPP