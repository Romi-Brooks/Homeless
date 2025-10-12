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

	public:
		DeadBody()
		: NPC(static_cast<NPCName>(hl_npc::NPCName::NPC_DeadBody), NPCType::NPC_Negative,
			NPCKillable::NPC_Killable, hl_npc::npc_bloody::NPC_DeadBody,
			hl_npc::npc_attack::NPC_DeadBody, hl_npc::npc_move_speed::NPC_DeadBody,hl_npc::npc_move_acceleration::NPC_DeadBody) {
			LOG_INFO(engine::log::LogChannel::GAME_MAIN, "Generated DeadBody");
		}

};


#endif // HOMELESS_DEADBODY_HPP
