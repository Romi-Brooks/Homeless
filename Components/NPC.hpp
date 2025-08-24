#ifndef HOMELESS_NPC_HPP
#define HOMELESS_NPC_HPP

#include "Entity.hpp"
#include "../Type/Type.hpp"
#include "../Engine/Random.hpp"


class NPC : public Entity{
	private:
		hl_npc::NPCType npcType_;
		hl_npc::NPCName npcName_;
		hl_npc::NPCKillable npcKillable_;

		sf::Clock moveClock_;

		bool isCollidingWithPlayer_ = false;

		float moveInterval_ = 0.5f; // 移动间隔不变

		auto setProperty() -> bool {
			switch (this->npcName_) {
				case hl_npc::NPCName::NPC_Name_DeadBody: {
					this->npcKillable_ = hl_npc::NPCKillable::NPC_Killable;
					this->npcType_ = hl_npc::NPCType::NPC_Type_Negative;
					return true;
				}
				default: {
					return false;
				}
			}
		}

		auto GetRandomDirection() -> Movement {
				// rand move direction
				int dir = GlobalRandom::GetRandomValue<std::uniform_int_distribution<int>>(0, 3);
				switch (dir) {
				case 0: return Movement::Entity_MoveUp;
				case 1: return Movement::Entity_MoveDown;
				case 2: return Movement::Entity_MoveLeft;
				default: return Movement::Entity_MoveRight;
				}
		}

	public:
		NPC(const hl_npc::NPCName name, const unsigned int hp, const unsigned int attack) :
			Entity(hp, attack), npcName_(name) {
			this->setProperty();

			this->CreateCircleWithColor(15, sf::Color::Red);


		};

		auto UpdateMovement(const sf::Vector2f& playerPos) -> void {
			if (moveClock_.getElapsedTime().asSeconds() < moveInterval_) {
				return;
			}
			moveClock_.restart();

			Movement moveDir;
			// 当是敌对NPC且与玩家判定圈碰撞时，优先向玩家移动
			if (npcType_ == hl_npc::NPCType::NPC_Type_Negative && isCollidingWithPlayer_) {
				// 提高向玩家移动的概率到90%
				int chance = GlobalRandom::GetRandomValue<std::uniform_int_distribution<int>>(0, 9);
				if (chance < 9) {
					moveDir = CalculateDirectionToPlayer(playerPos);
				} else {
					moveDir = GetRandomDirection();
				}
			} else {
				moveDir = GetRandomDirection();
			}

			Move(moveDir);
		}

		// 朝向玩家的方向
		auto CalculateDirectionToPlayer(const sf::Vector2f& playerPos) const -> Movement {
			sf::Vector2f npcPos = GetPosition();
			float xDiff = playerPos.x - npcPos.x;
			float yDiff = playerPos.y - npcPos.y;

			// 比较x和y方向的差距，优先向差距大的方向移动
			if (std::abs(xDiff) > std::abs(yDiff)) {
				return (xDiff > 0) ? Movement::Entity_MoveRight : Movement::Entity_MoveLeft;
			} else {
				return (yDiff > 0) ? Movement::Entity_MoveDown : Movement::Entity_MoveUp;
			}
		}

		// 与判定圈的碰撞状态
		void SetPlayerCollisionStatus(bool isColliding) {
				isCollidingWithPlayer_ = isColliding;
			}


		auto Move(const Movement Signal) const -> void override {
			auto x = this->shape_->getPosition().x;
			auto y = this->shape_->getPosition().y;
			float acceleration;

			switch (this->npcName_) {
				case hl_npc::NPCName::NPC_Name_DeadBody: {
					acceleration = hl_npc::move_speed::NPC_Name_DeadBody;
					break;
				}
				default: {
					acceleration = 0;
					break;
				}
			}

			switch (Signal) {
			case Movement::Entity_MoveLeft: {
				this->shape_->setPosition({x - (1 + acceleration), y});
				break;
			}
			case Movement::Entity_MoveRight: {
				this->shape_->setPosition({x + (1 + acceleration), y});
				break;
			}
			case Movement::Entity_MoveUp: {
				this->shape_->setPosition({x, y - (1 + acceleration)});
				break;
			}
			case Movement::Entity_MoveDown: {
				this->shape_->setPosition({x, y + (1 + acceleration)});
				break;
			}
			default: break;
			}
		}

		[[nodiscard]] auto GetNPCName() const {
			return this->npcName_;
		}

		[[nodiscard]] auto GetNPCType() const {
				return this->npcType_;
		}

		[[nodiscard]] auto GetNPCKillable() const {
				return this->npcKillable_;
		}
};


#endif //HOMELESS_NPC_HPP