#ifndef HOMELESS_POSITION_HPP
#define HOMELESS_POSITION_HPP

#include <random>
#include "Random.hpp"
#include "../Components/NPC.hpp"


class Position {
	private:

	public:
		// RNG
		static auto MakeNPCPositionRandom(NPC& npc, const sf::Window& renderWindow) -> void {
			const sf::Vector2u windowSize = renderWindow.getSize();

			// edge clip
			float x = GlobalRandom::GetRandomValue<std::uniform_real_distribution<float>>(0.0f, windowSize.x);
			float y = GlobalRandom::GetRandomValue<std::uniform_real_distribution<float>>(0.0f, windowSize.y);

			npc.setPosition(x, y);
		}
};



#endif //HOMELESS_POSITION_HPP