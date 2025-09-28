/**
  * @file           : NPC.hpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/9/23
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#ifndef HOMELESS_NPCPROPERTY_HPP
#define HOMELESS_NPCPROPERTY_HPP
namespace engine::npc_property {
	enum class NPCType {
		NPC_Negative,
		NPC_Neutral,
		NPC_Positive
	};

	enum class NPCKillable {
		NPC_Killable,
		NPC_Unkillable
	};
} // namespace engine::npc_property

#endif // HOMELESS_NPCPROPERTY_HPP
