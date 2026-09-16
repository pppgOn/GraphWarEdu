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

		playerUsed.AddCharacter(position);
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

	bool Map::CheckObstaclesHit(std::pair<float,float> position) {
		for (const Entity obstacle : m_obstacles) {
			if (obstacle.IsHitBy(position)) {
				m_explosionsDone.push_back(position);
				return true;
			}
		}

		return false;
	}

	bool Map::CheckPlayerHit(std::pair<float,float> position, std::pair<float,float> &positionHit, const Character &characterShouting) {
		for (const Character charachter : m_playerOne.m_charachters) {
			if (characterShouting == charachter) {
				continue;
			}
			
			if (charachter.IsHitBy(position)) {
				m_playerOne.m_charachters.remove(charachter);
				positionHit = charachter.m_position;
				return true;
			}
		}

		for (const Character enemy : m_playerTwo.m_charachters) {
			if (enemy.IsHitBy(position)) {
				m_playerTwo.m_charachters.remove(enemy);
				positionHit = enemy.m_position;
				return true;
			}
		}

		return false;
	}
}