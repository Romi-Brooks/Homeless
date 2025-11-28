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

#include <Windows/Screen.hpp>
#include <Windows/RenderWindow.hpp>

class HUD : public engine::window::screen::Screen {
	private:
		sf::Font font;
		std::vector<sf::Text> menuItems;
		float marginLeftRatio = 0.05f;  // 左对齐边距（相对于窗口宽度的比例）
		float lineSpacingRatio = 0.02f; // 行间距（相对于窗口高度的比例）
		float baseFontSizeRatio = 0.03f;// 基础字体大小（相对于窗口高度的比例）
		sf::Vector2u windowSize;        // 当前窗口大小
		sf::Vector2f scaleFactor = {1.0f, 1.0f}; // 缩放因子
		int selectedItem = 0;           // 当前选中的菜单项

	public:
		void Render(sf::RenderWindow& window) override {
			// 确保窗口大小被正确初始化
			if (windowSize.x == 0 || windowSize.y == 0) {
				windowSize = window.getSize();
			}

			for (const auto& text : menuItems) {
				window.draw(text);
			}
		}

		bool HandleEvent(const sf::Event& event) override {
			// 鼠标移动事件：只在鼠标悬停在菜单项上时处理，但不消费（允许下层屏幕也响应移动）
			if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
				const sf::Vector2f mousePos(
					static_cast<float>(mouseMoved->position.x),
					static_cast<float>(mouseMoved->position.y)
				);
				for (size_t i = 0; i < menuItems.size(); ++i) {
					if (menuItems[i].getGlobalBounds().contains(mousePos)) {
						selectedItem = i;
						break;
					}
				}
				return false; // 不消费移动事件，下层可响应
			}
			// 其他事件（如键盘）按需处理
			return false;
		}

		void Update(float deltaTime) override {
		}
};


#endif // HOMELESS_HUD_HPP
