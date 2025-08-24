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

	namespace move_speed{
		inline float NPC_Name_DeadBody = 1.5;
	}
}

namespace hl_player {


}

namespace hl_trans {
	inline auto GetNPCName(const hl_npc::NPCName name) -> std::string {
		switch (name) {
			case hl_npc::NPCName::NPC_Name_DeadBody: {
				return "NPC_Dead_Body";
			}
			default: {
				return "";
			}
		}
	}
	inline auto GetNPCType(const hl_npc::NPCType type) -> std::string {
		switch (type) {
		case hl_npc::NPCType::NPC_Type_Negative: {
			return "NPC_Negative";
		}
		case hl_npc::NPCType::NPC_Type_Neutral: {
			return "NPC_Neutral";
		}
		case hl_npc::NPCType::NPC_Type_Positive: {
			return "NPC_Positive";
		}
		default: {
			return "";
		}
		}
	}
	inline auto GetNPCKillable(const hl_npc::NPCKillable killable) -> std::string {
		switch (killable) {
		case hl_npc::NPCKillable::NPC_Killable: {
			return "NPC_Killable";
		}
		case hl_npc::NPCKillable::NPC_Unkillable: {
			return "NPC_UnKillable";
		}
		default: {
			return "";
		}
		}
	}
}
#endif //HOMELESS_TYPE_HPP