/**
  * @file           : Screen.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/28
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_SCREEN_HPP
#define HOMELESS_SCREEN_HPP

#include <SFML/Graphics/RenderWindow.hpp>

namespace engine::window::screen{
	class Screen {
		public:
			virtual ~Screen() = default;

			virtual auto Render(sf::RenderWindow& window) -> void = 0;
			virtual auto HandleEvent(const sf::Event& event) -> bool = 0;	// Use Boolean return values to ensure events are handled correctly.
			virtual auto Update(float deltaTime) -> void = 0;

			virtual auto OnActivate() -> void {}    // When active
			virtual auto OnDeactivate() -> void {}  // When switch out
	};
}

#endif // HOMELESS_SCREEN_HPP
