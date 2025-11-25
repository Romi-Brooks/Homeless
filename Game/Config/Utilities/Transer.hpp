#ifndef HOMELESS_TRANSER_HPP
#define HOMELESS_TRANSER_HPP

#include <string>

#include <Components/Entities/Entity.hpp>
#include <Config/Components/Entities/NPC.hpp>

#include "../Game/Config/Entities/NPC.hpp"

namespace hl_trans {
	inline auto GetNPCName(const game::config::hl_npc::NPCName name) -> std::string {
		switch (name) {
			case game::config::hl_npc::NPCName::NPC_DeadBody: {
				return "NPC_Dead_Body";
			}
			default: {
				return "";
			}
		}
	}
	inline auto GetNPCType(const engine::config::npc_property::NPCType type) -> std::string {
		switch (type) {
			case engine::config::npc_property::NPCType::NPC_Negative: {
				return "NPC_Negative";
			}
			case engine::config::npc_property::NPCType::NPC_Neutral: {
				return "NPC_Neutral";
			}
			case engine::config::npc_property::NPCType::NPC_Positive: {
				return "NPC_Positive";
			}
			default: {
				return "";
			}
		}
	}
	inline auto GetNPCKillable(const engine::config::npc_property::NPCKillable killable) -> std::string {
		switch (killable) {
			case engine::config::npc_property::NPCKillable::NPC_Killable: {
				return "NPC_Killable";
			}
			case engine::config::npc_property::NPCKillable::NPC_Unkillable: {
				return "NPC_UnKillable";
			}
			default: {
				return "";
			}
		}
	}
}

#endif //HOMELESS_TRANSER_HPP