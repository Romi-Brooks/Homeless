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
		}

		bool HandleEvent(const sf::Event& event) override {
		}

		void Update(float deltaTime) override {
		}
};


#endif // HOMELESS_HUD_HPP
