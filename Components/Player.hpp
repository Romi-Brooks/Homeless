#ifndef HOMELESS_PLAYER_HPP
#define HOMELESS_PLAYER_HPP

#include <string>
#include "Entity.hpp"
#include "../Type/Type.hpp"

class Player : public Entity {
	private:
		std::string name_;
		unsigned int money_;
		hl_player::PlayerLevel level_ = hl_player::PlayerLevel::Player_Level_1;

		auto setAttack() -> bool {
			this->attack_ = static_cast<unsigned>(this->level_) * 7;
			return true;
		}

	public:
		explicit Player(const std::string& name) :
			Entity(100, 0), name_(name), money_(50) {
			this->setAttack();

			this->CreateCircleWithColor(20, {255,255,255});
		};

		auto GetPlayerName() {
			return this->name_;
		}
};

#endif //HOMELESS_PLAYER_HPP