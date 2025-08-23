#ifndef HOMELESS_NPC_HPP
#define HOMELESS_NPC_HPP

#include "Entity.hpp"
#include "../Type/Type.hpp"

class NPC : public Entity{
	private:
		hl_npc::NPCType npcType_;
		hl_npc::NPCName npcName_;
		hl_npc::NPCKillable npcKillable_;

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

	public:
		NPC(const hl_npc::NPCName name, const unsigned int hp, const unsigned int attack) :
			Entity(hp, attack), npcName_(name) {
			this->setProperty();

			this->CreateCircleWithColor(10, {144,144,144});
		};

		auto GetNPCName() const {
			return this->npcName_;
		}
};


#endif //HOMELESS_NPC_HPP