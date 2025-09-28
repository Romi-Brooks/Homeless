/**
  * @file           : DeadBody.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/23
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_DEADBODY_HPP
#define HOMELESS_DEADBODY_HPP
#include "../../../Engine/Entities/NPC.hpp"
#include "../../Config/Entities/NPC.hpp"


class DeadBody : public NPC {
	private:
		hl_npc::NPCName name_;
		unsigned bloody;
		unsigned attack;

	public:
		DeadBody() :
			NPC(bloody, attack), name_(hl_npc::NPCName::NPC_DeadBody), bloody(hl_npc::npc_bloody::NPC_DeadBody), attack(hl_npc::npc_attack::NPC_DeadBody) {}

};


#endif // HOMELESS_DEADBODY_HPP
