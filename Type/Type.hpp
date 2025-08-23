#ifndef HOMELESS_TYPE_HPP
#define HOMELESS_TYPE_HPP

#include <string>

namespace hl_type{
	using uint8 = unsigned;
	using string = std::string;
}

namespace hl_npc {
	enum class NPCType {
		NPC_Type_Negative,
		NPC_Type_Neutral,
		NPC_Type_Positive
	};

	enum class NPCName {
		NPC_Name_DeadBody,
	};

	enum class NPCBloody {
		NPC_Bloody_DeadBody = 35,
	};

	enum class NPCAttack {
		NPC_Bloody_DeadBody = 5,
	};

	enum class NPCKillable {
		NPC_Killable,
		NPC_Unkillable
	};
}

namespace hl_player {
	enum class PlayerLevel {
		Player_Level_1 = 1,
		Player_Level_2,
		Player_Level_3
	};

}

namespace hl_name {
	inline auto GetNPCName(const hl_npc::NPCName name) -> std::string {
		switch (name) {
			case hl_npc::NPCName::NPC_Name_DeadBody: {
				return "Dead Body";
			}
			default: {
				return "";
			}
		}
	}
}
#endif //HOMELESS_TYPE_HPP