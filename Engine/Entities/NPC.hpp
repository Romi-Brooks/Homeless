#ifndef HOMELESS_NPC_HPP
#define HOMELESS_NPC_HPP

#include "Entity.hpp"
#include "../Config/Entities/NPC.hpp"
#include "../Log/LogSystem.hpp"

class NPC : public Entity{
	private:
		// NPC Types
		hl_npc::NPCType npcType_ {};
		hl_npc::NPCName npcName_ {};
		hl_npc::NPCKillable npcKillable_ {};

		// NPC Movement
		float moveSpeed_ {};
		float moveAcceleration_ {};

		auto SetProperty() -> bool {
			switch (this->npcName_) {
				case hl_npc::NPCName::NPC_DeadBody: {
					this->npcKillable_ = hl_npc::NPCKillable::NPC_Killable;
					this->npcType_ = hl_npc::NPCType::NPC_Negative;
					this->moveSpeed_ = hl_npc::npc_move_speed::NPC_DeadBody;
					this->moveAcceleration_ = hl_npc::npc_move_acceleration::NPC_DeadBody;
					return true;
				}
				default: {
					return false;
				}
			}
		}

	public:
		NPC(const hl_npc::NPCName name, const unsigned int hp, const unsigned int attack) :
			Entity(hp, attack), npcName_(name) {
			this->SetProperty();

			// Give a test Shape
			this->CreateCircleWithColor(15, sf::Color::Red);


		};

		auto Move(const Movement Signal) const -> void override {
			auto x = this->shape_->getPosition().x;
			auto y = this->shape_->getPosition().y;

			switch (Signal) {
				case Movement::Entity_MoveLeft: {
					this->shape_->setPosition({x - (this->moveSpeed_ + this->moveAcceleration_), y});
					break;
				}
				case Movement::Entity_MoveRight: {
					this->shape_->setPosition({x + (this->moveSpeed_ + this->moveAcceleration_), y});
					break;
				}
				case Movement::Entity_MoveUp: {
					this->shape_->setPosition({x, y - (this->moveSpeed_ + this->moveAcceleration_)});
					break;
				}
				case Movement::Entity_MoveDown: {
					this->shape_->setPosition({x, y + (this->moveSpeed_ + this->moveAcceleration_)});
					break;
				}
				default: {
					Log::LogOut("Error Movement Sign!", LogLevel::HL_ERROR);
					break;
				}
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