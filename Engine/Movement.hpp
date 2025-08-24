//
// Created by Romi on 2025/8/23.
//

#ifndef HOMELESS_EDGE_HPP
#define HOMELESS_EDGE_HPP

#include <iostream>
#include "../Components/Entity.hpp"



inline auto Move(const Entity& entity, const sf::Window& window) {
	float radius = entity.GetRadius();
	auto position = entity.GetShape()->getPosition();
	const auto windowSize = window.getSize();

		// Edge Detect
		bool hitBoundary = false;

		if (position.x < 0) {
			entity.GetShape()->setPosition({0, position.y});
			std::cout << "Move to x left edge, set x to 0!" << std::endl;
			hitBoundary = true;
		}
		else if (position.x + 2 * radius > windowSize.x) {
			entity.GetShape()->setPosition({windowSize.x - 2 * radius, position.y});
			std::cout << "Move to x right edge, set x to " << windowSize.x << "!" << std::endl;
			hitBoundary = true;
		}

		if (position.y < 0) {
			entity.GetShape()->setPosition({position.x, 0});
			std::cout << "Move to y top edge, set y to 0!" << std::endl;
			hitBoundary = true;
		}
		else if (position.y + 2 * radius > windowSize.y) {
			entity.GetShape()->setPosition({position.x, windowSize.y - 2 * radius});
			std::cout << "Move to y down edge, set y to " << windowSize.y << "!" << std::endl;
			hitBoundary = true;
		}

		// Movement
		if (!hitBoundary) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
				entity.Move(Movement::Entity_MoveRight);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
				entity.Move(Movement::Entity_MoveLeft);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
				entity.Move(Movement::Entity_MoveUp);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
				entity.Move(Movement::Entity_MoveDown);
			}
		}
}
#endif //HOMELESS_EDGE_HPP