#include "Map.h"

namespace gw {
		Map::Map(MapLimit limit) :
			m_limit(limit),
			m_size({limit.maxX - limit.minX, limit.maxY - limit.minY})
		{

		}

		bool Map::AddPlayerCharacter(Player &playerUsed, std::pair<float,float> position) {
			for (const Player player : {m_playerOne, m_playerTwo}) {
				for (const Entity character : player.m_charachters) {
					if (character.IsHitBy(position)) {
						return false;
					}
				}
			}

			playerUsed.AddCharater(position);
			return true;
		}

		bool Map::AddPlayerOneCharacter(std::pair<float,float> position) {
			return AddPlayerCharacter(m_playerOne, position);
		}

		bool Map::AddPlayerTwoCharacter(std::pair<float,float> position) {
			return AddPlayerCharacter(m_playerTwo, position);
		}

		void Map::AddObstacle(std::pair<float,float> position, float radius) {
			m_obstacles.push_back(Entity(position, radius));
		}
}