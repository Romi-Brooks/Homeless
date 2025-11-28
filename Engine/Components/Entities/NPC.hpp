/**
  * @file           : NPC.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/20
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_NPC_HPP
#define HOMELESS_NPC_HPP

// Self Dependencies
#include "Entity.hpp"
#include "../../Config/Components/Entities/NPC.hpp"
#include "../../Config/Movement/MoveEvent.hpp"
#include "../../Log/LogSystem.hpp"

enum class NPCName;

class NPC : public engine::entity::Entity{
	protected:
		// NPC Properties
		NPCName name_;

		// NPC Types
		engine::config::npc_property::NPCType npcType_ {};
		engine::config::npc_property::NPCKillable npcKillable_ {};

	public:
		NPC(const NPCName name, const engine::config::npc_property::NPCType type, const engine::config::npc_property::NPCKillable killable, const float hp, const float attack, const float moveSpeed, const float moveAcceleration)
		: Entity(hp, attack, moveSpeed, moveAcceleration) {
			name_ = name;
			npcType_ = type;
			npcKillable_ = killable;
			// Give a test Shape
			// For Test Only
			this->CreateCircleWithColor(15, sf::Color::Red);
		};

		auto Move(const engine::config::movements::Movement Signal) const -> void override {
			auto x = this->shape_->getPosition().x;
			auto y = this->shape_->getPosition().y;

			switch (Signal) {
				case engine::config::movements::Movement::Entity_MoveLeft: {
					this->shape_->setPosition({x - (this->moveSpeed_ + this->moveAcceleration_), y});
					break;
				}
				case engine::config::movements::Movement::Entity_MoveRight: {
					this->shape_->setPosition({x + (this->moveSpeed_ + this->moveAcceleration_), y});
					break;
				}
				case engine::config::movements::Movement::Entity_MoveUp: {
					this->shape_->setPosition({x, y - (this->moveSpeed_ + this->moveAcceleration_)});
					break;
				}
				case engine::config::movements::Movement::Entity_MoveDown: {
					this->shape_->setPosition({x, y + (this->moveSpeed_ + this->moveAcceleration_)});
					break;
				}
				default: {
					LOG_ERROR(engine::log::LogChannel::ENGINE_CONFIG_MOVEMENT,"Error Movement sign!");
					break;
				}
			}
		}

		[[nodiscard]] virtual auto GetNPCName() const -> std::string {
			// for subclass
			return "";
		}
		[[nodiscard]] auto GetNPCType() const {
				return this->npcType_;
		}

		[[nodiscard]] auto GetNPCKillable() const {
				return this->npcKillable_;
		}
};


#endif //HOMELESS_NPC_HPP