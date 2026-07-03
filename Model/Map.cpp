#include "Map.h"

namespace gw {
		Map::Map(MapSize size) :
			m_size(size)
		{

		}

		bool Map::AddPlayerCharacter(Player &playerUsed, std::pair<double,double> position) {
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

		bool Map::AddPlayerOneCharacter(std::pair<double,double> position) {
			return AddPlayerCharacter(m_playerOne, position);
		}

		bool Map::AddPlayerTwoCharacter(std::pair<double,double> position) {
			return AddPlayerCharacter(m_playerOne, position);
		}

		void Map::AddObstacle(std::pair<double,double> position, double radius) {
			m_obstacles.push_back(Entity(position, radius));
		}
}