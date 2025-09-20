#ifndef HOMELESS_GENERATE_HPP
#define HOMELESS_GENERATE_HPP

#include <vector>

#include "../../Entities/Player.hpp"
#include "../../Entities/NPC.hpp"
#include "../../Utilities/Random.hpp"

#include "../../Config/Entities/NPC.hpp"
#include "../../Config/Utilities/Transer.hpp"

#include "../../Log/LogSystem.hpp"

class Generate {
	private:
		static auto MakeNPCPositionRandom(const NPC& npc, const sf::Window& renderWindow) -> void {
			const sf::Vector2u windowSize = renderWindow.getSize();

			// edge clip
			float x = GlobalRandom::GetRandomValue<std::uniform_real_distribution<float>>(0.0f, windowSize.x);
			float y = GlobalRandom::GetRandomValue<std::uniform_real_distribution<float>>(0.0f, windowSize.y);

			npc.setPosition(x, y);
		}
		static auto RandomNPCPosition(std::vector<NPC>& npcs, const hl_npc::NPCName npcName, const sf::Window& renderWindow) {
				NPC& newNpc = npcs.back();
				Generate::MakeNPCPositionRandom(newNpc, renderWindow);
				Log::LogOut( "Set " + hl_trans::GetNPCName(npcName) + "'s Position to: " + std::to_string(newNpc.GetPosition().x) + "," + std::to_string(newNpc.GetPosition().y), LogLevel::HL_INFO);
		}

	public:
		static auto GenerateNPC(std::vector<NPC>& npcs, const hl_npc::NPCName npcName, const sf::Window& renderWindow) -> void {
			switch (npcName) {
				case hl_npc::NPCName::NPC_DeadBody: {
					npcs.reserve(npcs.size() + 1);
					npcs.emplace_back(
						hl_npc::NPCName::NPC_DeadBody,
						static_cast<unsigned>(hl_npc::npc_bloody::NPC_DeadBody),
						static_cast<unsigned>(hl_npc::npc_attack::NPC_DeadBody)
					);

					Generate::RandomNPCPosition(npcs, npcName, renderWindow);
					break;
				}
				default: { break; }
			}

		}

		static auto GeneratePlayer(std::vector<Player>& players, const std::string& name,  const sf::Window& renderWindow) -> void {
			players.reserve(players.size() + 1);
			players.emplace_back(name);

			const Player& newPlayer = players.back();
			newPlayer.setPosition(static_cast<float>(renderWindow.getSize().x) / 2, static_cast<float>(renderWindow.getSize().y) / 2);
		}
};


#endif //HOMELESS_GENERATE_HPP