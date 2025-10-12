// /**
//   * @file           : EdgeDetect.hpp
//   * @author         : Romi Brooks
//   * @brief          :
//   * @attention      :
//   * @date           : 2025/9/16
//   Copyright (c) 2025 Romi Brooks, All rights reserved.
// **/
//
// #ifndef HOMELESS_EDGEDETECT_HPP
// #define HOMELESS_EDGEDETECT_HPP
//
// #include "../../Entities/Entity.hpp"
//
// inline auto Move(const Entity& entity, const sf::Window& window) -> bool {
// 	float radius = entity.GetRadius();
// 	auto position = entity.GetShape()->getPosition();
// 	const auto windowSize = window.getSize();
//
// 	// Edge Detect
// 	bool hitBoundary = false;
//
// 	if (position.x < 0) {
// 		entity.GetShape()->setPosition({0, position.y});
// 		std::cout << "Move to x left edge, set x to 0!" << std::endl;
//
// 		hitBoundary = true;
// 	}
// 	else if (position.x + 2 * radius > windowSize.x) {
// 		entity.GetShape()->setPosition({windowSize.x - 2 * radius, position.y});
// 		std::cout << "Move to x right edge, set x to " << windowSize.x << "!" << std::endl;
// 		hitBoundary = true;
// 	}
//
// 	if (position.y < 0) {
// 		entity.GetShape()->setPosition({position.x, 0});
// 		std::cout << "Move to y top edge, set y to 0!" << std::endl;
// 		hitBoundary = true;
// 	}
// 	else if (position.y + 2 * radius > windowSize.y) {
// 		entity.GetShape()->setPosition({position.x, windowSize.y - 2 * radius});
// 		"Move to y down edge, set y to ", windowSize.y, "!" << std::endl;
// 		hitBoundary = true;
// 	}
//
//
// }
//
//
// #endif // HOMELESS_EDGEDETECT_HPP
