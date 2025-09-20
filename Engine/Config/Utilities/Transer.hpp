#ifndef HOMELESS_TRANSER_HPP
#define HOMELESS_TRANSER_HPP

#include <string>

#include "../Entities/NPC.hpp"

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

#endif //HOMELESS_TRANSER_HPP