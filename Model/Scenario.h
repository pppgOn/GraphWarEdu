#ifndef _Scenario_
#define _Scenario_

#include <utility>
#include <list>
#include "Map.h"

namespace gw {
	struct Scenario {
		MapLimit m_mapLimit;
		std::pair<double,double> m_player;
		std::list<std::pair<std::pair<double,double>,double>> m_obstacles;
	};

	extern Scenario linear;
	extern Scenario absolute;
	extern Scenario square;
}

#endif