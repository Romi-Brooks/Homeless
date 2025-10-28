/**
  * @file           : Player.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/20
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_PLAYER_HPP
#define HOMELESS_PLAYER_HPP

// Standard Libray
#include <string>
#include <utility>

// Self Dependencies
#include "Entity.hpp"
#include "../../Config/Movement/MoveEvent.hpp"

using PlayerMovement = engine::config::movements::Movement;

class Player final : public engine::entity::Entity {
	private:
		std::string name_;
		unsigned int money_;
		unsigned short level_ = 1;
		unsigned short exp_ = 0;

		// Detection
		sf::CircleShape detectionCircle_;
		float detectionRadius_ = 140;

		auto UpdateProperty() -> bool {
			this->attack_ = static_cast<float>(level_) * 7;
			return true;
		}

	public:
		Player(std::string name, const float hp, const float attack, const float moveSpeed, const float moveAcceleration) :
			Entity(hp, attack, moveSpeed, moveAcceleration), name_(std::move(name)), money_(50) {
			this->CreateCircleWithColor(20, {255,255,255});
		};


		auto Draw(sf::RenderWindow& window) const -> void override {
				Entity::Draw(window);
		}

		auto LevelUp() -> void {
			this->level_ = level_ + 1;
		}

		auto AddMoney(const unsigned int value) -> void {
			this->money_ = money_ + value;
		}

		auto AddExp(const unsigned int exp) -> void {
			this->exp_ = exp_ + exp;
		}


		auto Move(const PlayerMovement Signal) const -> void override {
			auto x = this->shape_->getPosition().x;
			auto y = this->shape_->getPosition().y;

			switch (Signal) {
			case PlayerMovement::Entity_MoveLeft: {
				this->shape_->setPosition({x - 3, y});
				break;
			}
			case PlayerMovement::Entity_MoveRight: {
				this->shape_->setPosition({x + 3, y});
				break;
			}
			case PlayerMovement::Entity_MoveUp: {
				this->shape_->setPosition({x, y - 3});
				break;
			}
			case PlayerMovement::Entity_MoveDown: {
				this->shape_->setPosition({x, y + 3});
				break;
			}
			default: break;
			}
		}

		[[nodiscard]] auto GetPlayerName() const {
			return this->name_;
		}

		[[nodiscard]] auto GetPlayerMoney() const {
			return this->money_;
		}

		[[nodiscard]] auto GetPlayerLevel() const {
			return this->level_;
		}

		[[nodiscard]] auto GetPlayerExp() const {
				return this->exp_;
		}

};

#endif //HOMELESS_PLAYER_HPP