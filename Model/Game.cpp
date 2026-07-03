#include "Game.h"

namespace gw {
	Game::Game(int playerOneCharactersNumber, int playerTwoCharactersNumber, double mapWitdth, double mapHeight) :
		m_map({-15, 15, -25, 25})
	{

	}

	Game::Game(Scenario screnario) :
		m_map(screnario.mapSize)
	{
		for (const std::pair<std::pair<double,double>,double> obstacle : screnario.obstacles) {
			m_map.AddObstacle(obstacle.first, obstacle.second);
		}
		m_map.AddPlayerOneCharacter(screnario.player);
	}
}