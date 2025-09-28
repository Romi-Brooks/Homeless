#ifndef HOMELESS_NPC_HPP
#define HOMELESS_NPC_HPP

#include "Entity.hpp"

#include "../Config/Entities/NPC.hpp"
#include "../Config/Movement/MoveEvent.hpp"
#include "../Log/LogSystem.hpp"

class NPC : public Entity{
	protected:
		// NPC Types
		engine::npc_property::NPCType npcType_ {};
		engine::npc_property::NPCKillable npcKillable_ {};

		// NPC Movement
		float moveSpeed_ {};
		float moveAcceleration_ {};

	public:
		NPC(const unsigned int hp, const unsigned int attack) : Entity(hp, attack) {
			// Give a test Shape
			this->CreateCircleWithColor(15, sf::Color::Red);
		};

		auto Move(const engine::Movement Signal) const -> void override {
			auto x = this->shape_->getPosition().x;
			auto y = this->shape_->getPosition().y;

			switch (Signal) {
				case engine::Movement::Entity_MoveLeft: {
					this->shape_->setPosition({x - (this->moveSpeed_ + this->moveAcceleration_), y});
					break;
				}
				case engine::Movement::Entity_MoveRight: {
					this->shape_->setPosition({x + (this->moveSpeed_ + this->moveAcceleration_), y});
					break;
				}
				case engine::Movement::Entity_MoveUp: {
					this->shape_->setPosition({x, y - (this->moveSpeed_ + this->moveAcceleration_)});
					break;
				}
				case engine::Movement::Entity_MoveDown: {
					this->shape_->setPosition({x, y + (this->moveSpeed_ + this->moveAcceleration_)});
					break;
				}
				default: {
					LOG_ERROR(LogChannel::ENGINE_MOVEMENT,"Error Movement sign!");
					break;
				}
			}
		}

		[[nodiscard]] auto GetNPCType() const {
				return this->npcType_;
		}

		[[nodiscard]] auto GetNPCKillable() const {
				return this->npcKillable_;
		}
};


#endif //HOMELESS_NPC_HPP