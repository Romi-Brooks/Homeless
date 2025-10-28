/**
  * @file           : Setting.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/10/13
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_SETTING_HPP
#define HOMELESS_SETTING_HPP

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <vector>

#include "../../Engine/Log/LogSystem.hpp"
#include "../../Engine/Windows/Manager/ScreenManager.hpp"

class Setting : public engine::window::screen::Screen {
	private:
		sf::Font font;
		std::vector<sf::Text> menuItems;
		float marginLeftRatio = 0.05f;  // 左对齐边距（相对于窗口宽度的比例）
		float lineSpacingRatio = 0.02f; // 行间距（相对于窗口高度的比例）
		float baseFontSizeRatio = 0.03f;// 基础字体大小（相对于窗口高度的比例）
		sf::Vector2u windowSize;        // 当前窗口大小
		sf::Vector2f scaleFactor = {1.0f, 1.0f}; // 缩放因子
		int selectedItem = 0;           // 当前选中的菜单项


		void InitMenuItems() {
			// 加载字体（确保字体文件路径正确）
			if (!font.openFromFile("Media/arial.ttf")) {
				// 处理字体加载失败的情况
				// 可以添加日志或默认字体
			}

			// 菜单选项
			const std::vector<std::string> en_items = {
				"Set FPS",
				"Music Volume",
				"SFX Volume",
				"Back"
			};


			// 创建文本对象
			for (const auto & item : en_items) {
				sf::Text text(font, item, 28); // 初始大小，后续会根据窗口调整
				text.setFillColor(sf::Color::White); // 设置文本颜色
				menuItems.push_back(text);
			}
		}

		void UpdateScaleFactor(sf::RenderWindow& window) {
			sf::Vector2u newSize = window.getSize();

			// 仅在窗口大小实际改变时才更新
			if (newSize != windowSize) {
				// 保存新窗口大小
				windowSize = newSize;
			}
		}

		// 左对齐布局实现，基于窗口比例
		void UpdateMenuPositions(sf::RenderWindow& window) {
			// 首先更新窗口大小
			UpdateScaleFactor(window);

			// 计算实际边距和间距（基于窗口比例）
			float marginLeft = marginLeftRatio * windowSize.x;
			float lineSpacing = lineSpacingRatio * windowSize.y;
			unsigned int fontSize = static_cast<unsigned int>(baseFontSizeRatio * windowSize.y);

			// 菜单起始Y坐标（垂直方向1/3处）
			const float startY = windowSize.y / 3.0f;

			for (size_t i = 0; i < menuItems.size(); ++i) {
				sf::Text& text = menuItems[i];
				// 设置字体大小
				text.setCharacterSize(fontSize);

				// 左对齐X坐标：左边距（不减去文本宽度）
				float x = marginLeft;

				// 垂直坐标计算：起始Y + 行索引 * (字体大小 + 行间距)
				float y = startY + i * (fontSize + lineSpacing);

				text.setPosition({x, y});

				// 高亮选中项
				if (i == selectedItem) {
					text.setFillColor(sf::Color::Yellow);
				} else {
					text.setFillColor(sf::Color::White);
				}
			}
		}

public:
	Setting() {
		InitMenuItems();
	}

	void Render(sf::RenderWindow& window) override {
		// 确保窗口大小被正确初始化
		if (windowSize.x == 0 || windowSize.y == 0) {
			windowSize = window.getSize();
		}

		// 更新菜单位置（确保窗口大小变化时也能正确显示）
		UpdateMenuPositions(window);

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

		// 鼠标点击事件：若点击了菜单项，则消费事件（避免下层响应）
		if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
			const sf::Vector2f mousePos(
				static_cast<float>(mouseButtonPressed->position.x),
				static_cast<float>(mouseButtonPressed->position.y)
			);
			// 检查点击是否在菜单项上
			for (size_t i = 0; i < menuItems.size(); ++i) {
				if (menuItems[i].getGlobalBounds().contains(mousePos)) {
					// 执行对应逻辑（如New Game、Exit等）
					switch (i) {
					case 0: break;
					case 1: break;
					case 2: break;
					case 3: {
						engine::window::manager::ScreenManager::GetInstance().SwitchScreen("Start");
						engine::window::manager::ScreenManager::GetInstance().PushScreen("Debugger");
						break;
					}
					}
					return true; // 消费点击事件，下层不再处理
				}
			}
			// 点击了空白区域，不消费
			return false;
		}
		// 其他事件（如键盘）按需处理
		return false;
	}

	void Update(float deltaTime) override {

	}
};


#endif // HOMELESS_SETTING_HPP
