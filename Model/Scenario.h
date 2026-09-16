#ifndef _Scenario_
#define _Scenario_

#include <utility>
#include <list>
#include "Map.h"

namespace gw {
	struct Scenario {
		MapLimit m_mapLimit;
		std::pair<float,float> m_player;
		std::list<std::pair<float,float>> m_enemies;
		std::list<std::pair<std::pair<float,float>,float>> m_obstacles;
	};

	extern Scenario linear;
	extern Scenario absolute;
	extern Scenario square;
}

#endif