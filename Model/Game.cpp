#include "Game.h"

namespace gw {
	Game::Game(int playerOneCharactersNumber, int playerTwoCharactersNumber, float mapWitdth, float mapHeight) :
		m_map({-15, 15, -25, 25})
	{

	}

	Game::Game(Scenario screnario) :
		m_map(screnario.m_mapLimit)
	{
		for (const std::pair<std::pair<float,float>,float> obstacle : screnario.m_obstacles) {
			m_map.AddObstacle(obstacle.first, obstacle.second);
		}
		m_map.AddPlayerOneCharacter(screnario.m_player);

		for (const std::pair<float,float> enemy : screnario.m_enemies) {
			m_map.AddPlayerTwoCharacter(enemy);
		}
	}
}