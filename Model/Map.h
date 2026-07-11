#ifndef _Map_
#define _Map_

#include <list>
#include "Entity.h"
#include "Player.h"

namespace gw {
	struct MapLimit {
		float minX;
		float maxX;
		float minY;
		float maxY;
	};

	struct MapSize {
		float width;
		float height;
	};

	class Map {
		public:
			Map(MapLimit mapLimit);
			bool AddPlayerOneCharacter(std::pair<float,float> position);
			bool AddPlayerTwoCharacter(std::pair<float,float> position);
			void AddObstacle(std::pair<float,float> position, float radius);

			MapLimit m_limit;
			MapSize m_size;
			Player m_playerOne;
			Player m_playerTwo;
			std::list<Entity> m_obstacles;
		
		private:
			bool AddPlayerCharacter(Player &player, std::pair<float,float> position);
	};
}

#endif
