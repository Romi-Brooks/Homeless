#ifndef HOMELESS_PLAYER_HPP
#define HOMELESS_PLAYER_HPP

#include <string>
#include <utility>

#include "Entity.hpp"
#include "../Config/Movement/MoveEvent.hpp"

class Player : public Entity {
	private:
		std::string name_;
		unsigned int money_;
		unsigned short level_ = 1;
		unsigned short exp_ = 0;

		// Movement
		float moveSpeed_ = 1;
		float moveAcceleration_ = 0;

		// Detection
		sf::CircleShape detectionCircle_;
		float detectionRadius_ = 140;

		auto UpdateProperty() -> bool {
			this->attack_ = level_ * 7;
			return true;
		}

	public:
		explicit Player(std::string name) :
			Entity(100, 7), name_(std::move(name)), money_(50) {
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


		auto Move(const engine::Movement Signal) const -> void override {
			auto x = this->shape_->getPosition().x;
			auto y = this->shape_->getPosition().y;

			switch (Signal) {
			case engine::Movement::Entity_MoveLeft: {
				this->shape_->setPosition({x - 3, y});
				break;
			}
			case engine::Movement::Entity_MoveRight: {
				this->shape_->setPosition({x + 3, y});
				break;
			}
			case engine::Movement::Entity_MoveUp: {
				this->shape_->setPosition({x, y - 3});
				break;
			}
			case engine::Movement::Entity_MoveDown: {
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