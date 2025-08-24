#ifndef HOMELESS_PLAYER_HPP
#define HOMELESS_PLAYER_HPP

#include <string>
#include "Entity.hpp"

class Player : public Entity {
	private:
		std::string name_;
		unsigned int money_;
		unsigned short level_ = 1;
		unsigned short exp_ = 0;

		sf::CircleShape detectionCircle_;
		float detectionRadius_ = 140;

		auto setAttack() -> bool {
			this->attack_ = static_cast<unsigned>(this->level_) * 7;
			return true;
		}

	public:
		explicit Player(const std::string& name) :
			Entity(100, 0), name_(name), money_(50) {
			this->setAttack();

			this->CreateCircleWithColor(20, {255,255,255});

			// 初始化判定圈（半透明绿色，不填充仅描边）
			detectionCircle_.setRadius(detectionRadius_);
			detectionCircle_.setFillColor(sf::Color::Transparent);
			detectionCircle_.setOutlineColor(sf::Color(0, 255, 0, 100));
			detectionCircle_.setOutlineThickness(2.f);

			// 关键修复：将判定圈原点设为中心（与玩家形状同步）
			detectionCircle_.setOrigin({detectionRadius_ - this->GetRadius(), detectionRadius_ - this->GetRadius()});
		};

		auto UpdateDetectionCircle() -> void {
			if (GetShape()) {
				detectionCircle_.setPosition(GetPosition());
			}
		}

		auto setPosition(float x, float y) -> bool override {
				bool success = Entity::setPosition(x, y);
				if (success) {
					detectionCircle_.setPosition({x, y});
				}
				return success;
		}

		auto IsNPCInRange(const Entity& npc) const -> bool {
				sf::Vector2f playerPos = GetPosition();
				sf::Vector2f npcPos = npc.GetPosition();

				float dx = npcPos.x - playerPos.x;
				float dy = npcPos.y - playerPos.y;
				float distanceSquared = dx * dx + dy * dy;

				// range sqr detect
				float rangeSquared = detectionRadius_ * detectionRadius_;
				return distanceSquared <= rangeSquared;
		}

		auto Draw(sf::RenderWindow& window) const -> void override {
				window.draw(detectionCircle_);	// detection line
				Entity::Draw(window); // circle
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


		auto Move(const Movement Signal) const -> void override {
			auto x = this->shape_->getPosition().x;
			auto y = this->shape_->getPosition().y;

			switch (Signal) {
			case Movement::Entity_MoveLeft: {
				this->shape_->setPosition({x - 3, y});
				break;
			}
			case Movement::Entity_MoveRight: {
				this->shape_->setPosition({x + 3, y});
				break;
			}
			case Movement::Entity_MoveUp: {
				this->shape_->setPosition({x, y - 3});
				break;
			}
			case Movement::Entity_MoveDown: {
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