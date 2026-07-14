#include "Game.h"

namespace gw {
	Game::Game(int playerOneCharactersNumber, int playerTwoCharactersNumber, double mapWitdth, double mapHeight) :
		m_map({-15, 15, -25, 25})
	{

	}

	Game::Game(Scenario screnario) :
		m_map(screnario.m_mapLimit)
	{
		for (const std::pair<std::pair<double,double>,double> obstacle : screnario.m_obstacles) {
			m_map.AddObstacle(obstacle.first, obstacle.second);
		}
		m_map.AddPlayerOneCharacter(screnario.m_player);

		for (const std::pair<double,double> enemy : screnario.m_enemies) {
			m_map.AddPlayerTwoCharacter(enemy);
		}
	}
}