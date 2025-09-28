// #ifndef HOMELESS_MAINWINDOW_HPP
// #define HOMELESS_MAINWINDOW_HPP
//
// #include <cstring>
//
// #include <SFML/Audio.hpp>
// #include <SFML/Graphics.hpp>
// #include <imgui.h>
// #include <imgui-SFML.h>
//
// #include "../../Engine/Entities/NPC.hpp"
// #include "../../Engine/Entities/Player.hpp"
//
//
// #include "../../Engine/Actions/Generation/Generate.hpp"
// #include "../../Engine/Entities/Entity.hpp"
//
// #include "../../Engine/Audio/Music.hpp"
// #include "../../Engine/Audio/MusicFade.hpp"
// #include "../../Engine/Audio/SFX.hpp"
// #include "../../Engine/Audio/SFXManager.hpp"
//
//
// class MainWindow {
// 	private:
// 		// sfml basic controller
// 		sf::RenderWindow *window_ = nullptr;
// 		sf::Texture *texture_ = nullptr;
// 		sf::Sprite *sprite_ = nullptr;
// 		sf::Font *font_ = nullptr;
// 		sf::Text *text_ = nullptr;
//
// 		sf::VertexArray connections_;
//
// 		sf::Clock deltaClock_;
//
// 		std::vector<NPC> npc_;
// 		std::vector<Player> player_;
// 		size_t selectedNPCIndex = -1;  // -1表示未选中
// 		size_t selectedPlayerIndex = -1;  // -1表示未选中
//
// 		// DPI zoom
// 		float dpiScale_ = 1.0f;
// 		unsigned int maxFrames_ = 120;
// 		bool isMusicLoad_ = false;
// 		bool isFullscreen_ = false;
// 		// bool showDemoWindow_ = true; // 控制是否显示ImGui演示窗口
//
// 		ImVec4 clearColor_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // 背景颜色
//
// 		// ImGui Text buffer
// 		char textInputBuffer[256] = ""; // 可以根据需要调整缓冲区大小
// 		bool textInputActive = false;   // 用于跟踪文本框是否处于活动状态
//
// 		auto DetectAndSetDPIScaling() -> void {
// 			// 获取主显示器的DPI
// 			sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
//
// 			// 简单估算DPI缩放因子
// 			// 标准DPI是96，如果屏幕宽度大于1920，则增加缩放
// 			if (desktopMode.size.x > 2560) {
// 				dpiScale_ = 2.0f; // 4K屏幕
// 			} else if (desktopMode.size.x > 1920) {
// 				dpiScale_ = 1.5f; // 2K屏幕
// 			} else {
// 				dpiScale_ = 1.0f; // 全高清
// 			}
//
// 			// 应用缩放到ImGui
// 			ImGui::GetStyle().ScaleAllSizes(dpiScale_);
// 			ImGui::GetIO().FontGlobalScale = dpiScale_;
//
// 			// 重新创建字体纹理以适应新的缩放
// 			if (ImGui::SFML::UpdateFontTexture()) {
// 				return;
// 			}
//
// 		}
//
// 	public:
// 		MainWindow(const unsigned int x, const unsigned int y, const std::string& Tittle) {
// 			this->InitWindow(x, y, Tittle);
// 			this->window_->setFramerateLimit(this->maxFrames_);
// 			this->InitFontTexture();
// 			this->InitSprite(x, y);
//
// 			this->SetupFadeCallbacks(); // music switch callback
//
// 			this->InitImGui();
// 			// this->DetectAndSetDPIScaling();		// there are some bugs, so don't use it before fixed it.
//
// 			this->npc_ = std::vector<NPC>();
// 			this->player_ = std::vector<Player>();
//
// 			this->connections_.setPrimitiveType(sf::PrimitiveType::Lines);
//
// 		}
//
// 		MainWindow(const unsigned int x, const unsigned int y, const std::string& Tittle, unsigned int Frames)
// 		: MainWindow(x, y, Tittle) {
// 			this->maxFrames_ = Frames;
// 			this->window_->setFramerateLimit(this->maxFrames_);
// 		}
//
// 		auto InitWindow(const unsigned int x, const unsigned int y, const std::string& Tittle) -> bool {
// 			this->window_ = new sf::RenderWindow(sf::VideoMode({x, y}), Tittle);
// 			return true;
// 		}
//
// 		auto InitFontTexture()-> bool {
// 			this->font_ = new sf::Font("Media/arial.ttf");
// 			return true;
// 		}
//
// 		auto InitImGui() const -> bool {
// 			// 初始化ImGui-SFML
// 			if (!ImGui::SFML::Init(*window_)) {
// 				return false;
// 			}
//
// 			// 设置ImGui样式
// 			ImGui::StyleColorsDark();
//
// 			// 获取ImGui样式引用
// 			ImGuiStyle& style = ImGui::GetStyle();
//
// 			// 根据DPI缩放调整样式
// 			style.ScaleAllSizes(dpiScale_);
//
// 			// 加载字体 - 使用更大的基础字体大小
// 			ImGuiIO& io = ImGui::GetIO();
//
// 			// 或者加载自定义字体大小
// 			ImFont* font = io.Fonts->AddFontFromFileTTF("Media/arial.ttf", 16.0f * dpiScale_);
// 			io.FontDefault = font;
//
// 			// 创建字体纹理
// 			if (!ImGui::SFML::UpdateFontTexture()) {
// 				return false;
// 			}
//
// 			return true;
//
// 		}
//
// 		auto InitSprite(const unsigned int x, const unsigned int y) -> bool {
// 			// first init texture
// 			this->texture_ = new sf::Texture(sf::Vector2u(x,y));
//
// 			// Set Sprite from texture
// 			this->sprite_ = new sf::Sprite(*this->texture_);
//
// 			return true;
// 		}
//
// 		auto LoadTexture(const std::string& filename) -> bool {
// 			if (this->texture_->loadFromFile(filename)) {
// 				this->sprite_ = new sf::Sprite(*this->texture_);
// 				return true;
// 			}
// 			return false;
// 		}
//
// 		auto LoadText(const std::string& text) -> bool {
// 			// 确保字体已加载
// 			if (this->font_ == nullptr) {
// 				LOG_ERROR(LogChannel::ENGINE_WINDOWS, "Font not loaded! Cannot create text.");
// 				return false;
// 			}
//
// 			// 如果已有文本对象，先删除
// 			if (this->text_ != nullptr) {
// 				delete this->text_;
// 				this->text_ = nullptr;
// 			}
//
// 			// 创建新的文本对象
// 			this->text_ = new sf::Text(*this->font_, text, static_cast<unsigned int>(50 * dpiScale_));
//
// 			// 设置文本位置（居中显示）
// 			this->text_->setPosition({this->window_->getSize().x / 2.0f,
// 									this->window_->getSize().y / 2.0f});
//
// 			// 设置文本颜色
// 			this->text_->setFillColor(sf::Color::White);
//
// 			// 添加轮廓（可选）
// 			this->text_->setOutlineColor(sf::Color::Black);
// 			this->text_->setOutlineThickness(2.0f);
//
// 			LOG_INFO(LogChannel::ENGINE_WINDOWS, "Text created: " + text);
// 			return true;
// 		}
//
// 		auto SetToFullscreen() -> void {
// 				// 切换到全屏模式
// 				this->InitSprite(sf::VideoMode::getFullscreenModes()[0].size.x,sf::VideoMode::getFullscreenModes()[0].size.y);
//
// 				this->window_->create(sf::VideoMode::getFullscreenModes()[0], "SFML + ImGui Test", sf::State::Fullscreen);
// 				sf::sleep(sf::milliseconds(100));
// 				this->window_->setFramerateLimit(this->maxFrames_);
//
// 				this->UpdateAfterResolutionChange();
//
// 				isFullscreen_ = true;
//
// 				LOG_INFO(LogChannel::ENGINE_WINDOWS, "Set to Fullscreen Mode!");
// 			}
//
// 		auto SetToWindowed() -> void {
// 				this->InitSprite(this->window_->getSize().x, this->window_->getSize().y);
//
// 				this->window_->create(sf::VideoMode({1920, 1080}), "SFML Window", sf::Style::Default);
// 				sf::sleep(sf::milliseconds(100));
// 				this->window_->setFramerateLimit(this->maxFrames_);
//
// 				this->UpdateAfterResolutionChange();
//
// 				isFullscreen_ = false;
//
// 				LOG_INFO(LogChannel::ENGINE_WINDOWS, "Set to Windowed Mode!");
// 			}
//
// 		auto UpdateAfterResolutionChange() const -> void {
// 				sf::View view = this->window_->getView();
// 				view.setSize({this->window_->getSize().x / dpiScale_, this->window_->getSize().y / dpiScale_});
// 				this->window_->setView(view);
//
// 				if (this->text_ != nullptr) {
// 					this->text_->setPosition({this->window_->getSize().x / 2.0f,
// 											this->window_->getSize().y / 2.0f});
// 				}
// 			}
//
// 		auto EntityRender() const -> void {
// 			for (auto& npc : this->npc_) {
// 				npc.Draw(*this->window_);
// 			}
// 		}
//
// 		static void SetupFadeCallbacks() {
// 				engine::sfx::MusicFade::GetInstance().SetCallback(
// 					[](const engine::sfx::MusicFade::FadeState state,
// 					   const std::string& fromId, const std::string& toId) {
// 						switch (state) {
// 							case engine::sfx::MusicFade::FadeState::FadingOut:
// 							LOG_INFO(LogChannel::ENGINE_AUDIO_MUSIC_FADE, "Fading out: " + fromId);
// 							break;
// 						case engine::sfx::MusicFade::FadeState::FadingIn:
// 							LOG_INFO(LogChannel::ENGINE_AUDIO_MUSIC_FADE, "Fading in: " + toId);
// 							break;
// 						case engine::sfx::MusicFade::FadeState::Completed:
// 							LOG_INFO(LogChannel::ENGINE_AUDIO_MUSIC_FADE, "Fade completed: " + fromId + " to " + toId);
// 							break;
// 						default:
// 							break;
// 					}
// 				}
// 			);
// 		}
//
// 		auto ClearNPC() -> void {
// 			this->npc_.clear();
// 		}
// 		auto ClearPlayer() -> void {
// 				this->player_.clear();
// 		}
//
// 		~MainWindow() {
// 			ImGui::SFML::Shutdown(); // 关闭ImGui
// 			delete window_;
// 			delete texture_;
// 			delete sprite_;
// 			delete font_;
// 		}
//
// 		auto Run() -> void {
// 		    while (this->window_->isOpen()) {
// 		        // 事件处理
// 		        while (const auto event = this->window_->pollEvent()) {
// 		            ImGui::SFML::ProcessEvent(*window_, *event);
// 		            if (event->is<sf::Event::Closed>()) {
// 		                this->window_->close();
// 		            }
// 		        }
//
// 		        // 更新ImGui
// 		        ImGui::SFML::Update(*this->window_, this->deltaClock_.restart());
// 		        this->CreateImGuiInterface();
//
// 		        // 清除屏幕
// 		        this->window_->clear();
//
//
// 		    	if (!this->player_.empty()) {
// 		    		auto& player = *this->player_.begin(); // 获取玩家实例
// 		    		sf::Vector2f playerPos = player.GetPosition(); // 玩家当前位置
//
//
// 		    		// 4.2 清空上一帧的连接线（避免重复绘制）
// 		    		connections_.clear();
// 		    	}
//
// 				// npc & player render
// 				this->EntityRender();
//
// 		        // 绘制其他元素
// 		        this->window_->draw(*this->sprite_);
// 		        if (this->text_ != nullptr) {
// 		            this->window_->draw(*this->text_);
// 		        }
//
// 		        // 渲染ImGui（必须在所有绘制之后）
// 		        ImGui::SFML::Render(*this->window_);
// 		        this->window_->display();
// 		    }
// 		}
//
// 		auto CreateImGuiInterface() -> void {
// 				// 1. 显示一个简单的窗口
// 				{
// 					ImGui::Begin("Homeless ImGui Debugger"); // 创建一个窗口
//
// 					// 添加文本
// 					ImGui::Text("Build Debugger with SFML3 and Imgui 1.92.2");
//
//
//
// 					// 动态切换按钮
// 					if (isFullscreen_) {
// 						// 当前是全屏模式，显示"切换到窗口模式"按钮
// 						if (ImGui::Button("Windowed")) {
// 							this->SetToWindowed();
// 						}
// 					} else {
// 						// 当前是窗口模式，显示"切换到全屏模式"按钮
// 						if (ImGui::Button("Fullscreen")) {
// 							this->SetToFullscreen();
// 						}
// 					}
//
//
// 					if (ImGui::Button("Load NPC_DeadBody")) {
// 						// Generate::GenerateNPC(this->npc_, hl_npc::NPCName::NPC_DeadBody, *this->window_);
//
// 					}
//
// 					if (ImGui::Button("Load Player")) {
// 						// Generate::GeneratePlayer(this->player_, "Romi", *this->window_);
// 					}
//
// 					if (ImGui::Button("Clear NPC&Player")) {
// 						this->ClearNPC();
// 						this->ClearPlayer();
// 					}
//
// 					if (ImGui::Button("Play Kick")) {
// 						engine::SFX::GetInstance().Play("Kick");
// 					}
//
// 					if (ImGui::Button("Play Snare")) {
// 						engine::SFX::GetInstance().Play("Snare");
// 					}
//
// 					if (ImGui::Button("Play Background")) {
// 						engine::Music::GetInstance().Play("bg1");
// 					}
//
// 					if (ImGui::Button("Switch Background")) {
// 						if (bool success = engine::sfx::MusicFade::GetInstance().Switch("bg1", "bg2", 3.0f)) {
// 							LOG_INFO(LogChannel::ENGINE_AUDIO_MUSIC_FADE, "Started fade transition from bg1 to bg2");
// 						}
// 					}
//
//
// 					// Load Text
// 					{
// 						ImGui::Begin("Text Loader");
//
// 						ImGui::Text("Text:");
//
// 						if (ImGui::InputText("##Input Label", textInputBuffer, IM_ARRAYSIZE(textInputBuffer),
// 											ImGuiInputTextFlags_EnterReturnsTrue))
// 						{
// 							// 当用户按下回车键时调用LoadText
// 							if (std::strlen(textInputBuffer) > 0) {
// 								this->LoadText(std::string(textInputBuffer));
// 							}
// 						}
//
// 						// 跟踪文本框是否处于活动状态
// 						textInputActive = ImGui::IsItemActive();
//
// 						// 添加一个按钮来提交文本
// 						if (ImGui::Button("Submit")) {
// 							if (std::strlen(textInputBuffer) > 0) {
// 								this->LoadText(std::string(textInputBuffer));
// 							}
// 						}
//
// 						// 添加一个清空按钮
// 						ImGui::SameLine();
// 						if (ImGui::Button("Clear")) {
// 							std::memset(textInputBuffer, 0, sizeof(textInputBuffer));
// 						}
//
// 						// 显示当前文本长度
// 						ImGui::Text("Length: %zu ch", std::strlen(textInputBuffer));
//
// 						ImGui::End();
// 					}
//
// 					// 添加帧率显示
// 					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
// 								1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//
// 					ImGui::End();
// 				}
//
// 				// 设置窗口背景颜色
// 				const sf::Color bgColor(
// 					static_cast<int8_t>(clearColor_.x * 255),
// 					static_cast<int8_t>(clearColor_.y * 255),
// 					static_cast<int8_t>(clearColor_.z * 255),
// 					static_cast<int8_t>(clearColor_.w * 255)
// 				);
//
// 				this->window_->clear(bgColor);
// 			}
//
//
//
// 		MainWindow(const MainWindow&) = delete;
// 		MainWindow& operator=(const MainWindow&) = delete;
// };
//
// #endif //HOMELESS_MAINWINDOW_HPP