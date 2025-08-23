#ifndef HOMELESS_MAINWINDOW_HPP
#define HOMELESS_MAINWINDOW_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "cstring"
#include "iostream"

#include "../ECS/Entity.hpp"

class MainWindow {
	private:
		// sfml basic controller
		sf::RenderWindow *window_ = nullptr;
		sf::Texture *texture_ = nullptr;
		sf::Sprite *sprite_ = nullptr;
		sf::Font *font_ = nullptr;
		sf::Text *text_ = nullptr;
		sf::Music *music_ = nullptr;
		sf::Clock deltaClock_;

		std::vector<Entity> shapes_;

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

			this->shapes_ = std::vector<Entity>(); // 初始化空向量
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

		auto InitImGui() -> bool {
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

		auto PlayMusic() -> bool {
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

		auto UpdateAfterResolutionChange() -> void {
				sf::View view = this->window_->getView();
				view.setSize({this->window_->getSize().x / dpiScale_, this->window_->getSize().y / dpiScale_});
				this->window_->setView(view);

				if (this->text_ != nullptr) {
					this->text_->setPosition({this->window_->getSize().x / 2.0f,
											this->window_->getSize().y / 2.0f});
				}
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
				// Process events
				while (const auto event = this->window_->pollEvent()) {
					// 将事件传递给ImGui
					ImGui::SFML::ProcessEvent(*window_, *event);

					// Close window: exit
					if (event->is<sf::Event::Closed>()) {
						this->window_->close();
					}
				}

					// 更新ImGui
					ImGui::SFML::Update(*this->window_, this->deltaClock_.restart());

					// 创建ImGui界面
					this->CreateImGuiInterface();

					// Clear screen
					this->window_->clear();

					for (auto& entity : shapes_) {
						entity.Draw(*window_);
					}


{
						if (!this->shapes_.empty()) {
							if (this->shapes_.begin()->GetShape()->getPosition().x < 0) {
								this->shapes_.begin()->GetShape()->setPosition({0, this->shapes_.begin()->GetShape()->getPosition().y});
								std::cout << "Move to x left edge, set x to 0!" << std::endl;
							}
							if (this->shapes_.begin()->GetShape()->getPosition().y < 0) {
								this->shapes_.begin()->GetShape()->setPosition({this->shapes_.begin()->GetShape()->getPosition().x, 0});
								std::cout << "Move to y top edge, set y to 0!" << std::endl;
							}
							if (this->shapes_.begin()->GetShape()->getPosition().x + (2 * this->shapes_.begin()->GetRadius()) > this->window_->getSize().x) {
								this->shapes_.begin()->GetShape()->setPosition({(this->window_->getSize().x - 2 * this->shapes_.begin()->GetRadius()),
																					this->shapes_.begin()->GetShape()->getPosition().y});
								std::cout << "Move to x right edge, set x to " << this->window_->getSize().x << "!" << std::endl;
							}
							if (this->shapes_.begin()->GetShape()->getPosition().y + (2 * this->shapes_.begin()->GetRadius()) > this->window_->getSize().y) {
								this->shapes_.begin()->GetShape()->setPosition({this->shapes_.begin()->GetShape()->getPosition().x,
																					(this->window_->getSize().y - 2 * this->shapes_.begin()->GetRadius())});
								std::cout << "Move to y down edge, set y to " << this->window_->getSize().y << "!" << std::endl;
							}
							else {
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
									this->shapes_.begin()->Move(MoveSig::RIGHT);
								}
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
									this->shapes_.begin()->Move(MoveSig::LEFT);
								}
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
									this->shapes_.begin()->Move(MoveSig::UP);
								}
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
									this->shapes_.begin()->Move(MoveSig::DOWN);
								}
							}
						}
					}

					// Draw the sprite
					this->window_->draw(*this->sprite_);

					if (this->text_ != nullptr) {
						this->window_->draw(*this->text_);
					}

					// 渲染ImGui
					ImGui::SFML::Render(*this->window_);

					// Update the window
					this->window_->display();
				}
			}

		auto CreateImGuiInterface() -> void {
				// 1. 显示一个简单的窗口
				{
					ImGui::Begin("Hello, SFML with ImGUI!"); // 创建一个窗口

					// 添加文本
					ImGui::Text("Build demo with SFML3 and Imgui 1.92.2");

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

					// if (ImGui::Button("Demo Window")) {
					// 	showDemoWindow_ = !showDemoWindow_;
					// }

					if (ImGui::Button("Load Player")) {
						this->shapes_.emplace_back(10.f, sf::Color(255, 255, 255));
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

						// 创建文本框
						// ImGuiInputTextFlags_EnterReturnsTrue 表示按下回车键时返回true
						if (ImGui::InputText("##Input Lable", textInputBuffer, IM_ARRAYSIZE(textInputBuffer),
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

				// // 3. 显示ImGui演示窗口（非常有用的调试工具）
				// if (showDemoWindow_) {
				// 	ImGui::ShowDemoWindow(&showDemoWindow_);
				// }

				// 设置窗口背景颜色
				sf::Color bgColor(
					static_cast<int8_t>(clearColor_.x * 255),
					static_cast<int8_t>(clearColor_.y * 255),
					static_cast<int8_t>(clearColor_.z * 255),
					static_cast<int8_t>(clearColor_.w * 255)
				);

				this->window_->clear(bgColor);
			}

		MainWindow(const MainWindow&) = delete;
		MainWindow& operator=(const MainWindow&) = delete;
};

#endif //HOMELESS_MAINWINDOW_HPP