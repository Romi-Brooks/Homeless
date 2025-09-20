#ifndef TYPE_NPC_HPP
#define TYPE_NPC_HPP

namespace hl_npc {
	enum class NPCType {
		NPC_Negative,
		NPC_Neutral,
		NPC_Positive
	};

	enum class NPCName {
		NPC_DeadBody,
		NPC_FatNinja,
	};

	enum class NPCKillable {
		NPC_Killable,
		NPC_Unkillable
	};

	namespace npc_bloody {
		inline unsigned NPC_DeadBody = 55;
		inline unsigned NPC_FatNinja = 80;
	}

	namespace npc_attack {
		inline float NPC_DeadBody = 3;
		inline float NPC_FatNinja = 2.5;
	}

	namespace npc_move_speed {
		inline float NPC_DeadBody = 1.5;
		inline float NPC_FatNinja = 1;
	}

	namespace npc_move_acceleration {
		inline float NPC_DeadBody = 0;
		inline float NPC_FatNinja = 1;
	}
}
#endif // TYPE_NPC_HPP
