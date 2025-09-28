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

			virtual void Render(sf::RenderWindow& window) = 0;
			virtual void HandleEvent(const sf::Event& event) = 0;
			virtual void Update(float deltaTime) = 0;

			virtual void OnActivate() {}    // When active
			virtual void OnDeactivate() {}  // When switch out
	};
}

#endif // HOMELESS_SCREEN_HPP
