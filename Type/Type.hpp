#ifndef HOMELESS_TYPE_HPP
#define HOMELESS_TYPE_HPP

#include <string>

namespace hl_type{
	using uint8 = unsigned;
	using string = std::string;
}

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

	namespace npc_move_speed{
		inline float NPC_DeadBody = 1.5;
	}
}

namespace hl_player {


}

namespace hl_trans {
	inline auto GetNPCName(const hl_npc::NPCName name) -> std::string {
		switch (name) {
			case hl_npc::NPCName::NPC_DeadBody: {
				return "NPC_Dead_Body";
			}
			default: {
				return "";
			}
		}
	}
	inline auto GetNPCType(const hl_npc::NPCType type) -> std::string {
		switch (type) {
		case hl_npc::NPCType::NPC_Negative: {
			return "NPC_Negative";
		}
		case hl_npc::NPCType::NPC_Neutral: {
			return "NPC_Neutral";
		}
		case hl_npc::NPCType::NPC_Positive: {
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