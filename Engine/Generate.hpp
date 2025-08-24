#ifndef HOMELESS_GENERATE_HPP
#define HOMELESS_GENERATE_HPP

#include <vector>

#include "Position.hpp"
#include "../Components/NPC.hpp"
#include "../Components/Player.hpp"
#include "../Log/LogSystem.hpp"
#include "../Type/Type.hpp"

class Generate {
	private:
		static auto RandomNPCPosition(std::vector<NPC>& npcs, const hl_npc::NPCName npcName, const sf::Window& renderWindow) {
				NPC& newNpc = npcs.back();
				Position::MakeNPCPositionRandom(newNpc, renderWindow);
				Log::LogOut( "Set " + hl_trans::GetNPCName(npcName) + "'s Position to: " + std::to_string(newNpc.GetPosition().x) + "," + std::to_string(newNpc.GetPosition().y), LogLevel::BP_INFO);
		}

	public:
		static auto GenerateNPC(std::vector<NPC>& npcs, const hl_npc::NPCName npcName, const sf::Window& renderWindow) -> void {
			switch (npcName) {
				case hl_npc::NPCName::NPC_Name_DeadBody: {
					npcs.reserve(npcs.size() + 1);
					npcs.emplace_back(
						hl_npc::NPCName::NPC_Name_DeadBody,
						static_cast<unsigned>(hl_npc::NPCBloody::NPC_Bloody_DeadBody),
						static_cast<unsigned>(hl_npc::NPCAttack::NPC_Bloody_DeadBody)
					);

					Generate::RandomNPCPosition(npcs, npcName, renderWindow);
					break;
				}
			}

		}

		static auto GeneratePlayer(std::vector<Player>& players, const std::string& name,  const sf::Window& renderWindow) -> void {
			players.reserve(players.size() + 1);
			players.emplace_back(name);

			Player& newPlayer = players.back();
			newPlayer.setPosition(renderWindow.getSize().x / 2, renderWindow.getSize().y / 2);
		}



};


#endif //HOMELESS_GENERATE_HPP