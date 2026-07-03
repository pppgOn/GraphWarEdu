#ifndef _Map_
#define _Map_

#include <list>
#include "Entity.h"
#include "Player.h"

namespace gw {
	struct MapSize {
		double minX;
		double maxX;
		double minY;
		double maxY;
	};

	class Map {
		public:
			Map(MapSize mapSize);
			bool AddPlayerOneCharacter(std::pair<double,double> position);
			bool AddPlayerTwoCharacter(std::pair<double,double> position);
			void AddObstacle(std::pair<double,double> position, double radius);

			MapSize m_size;
			Player m_playerOne;
			Player m_playerTwo;
			std::list<Entity> m_obstacles;
		
		private:
			bool AddPlayerCharacter(Player &player, std::pair<double,double> position);
	};
}

#endif
