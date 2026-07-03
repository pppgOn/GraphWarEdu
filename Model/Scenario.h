#ifndef _Scenario_
#define _Scenario_

#include <utility>
#include <list>
#include "Map.h"

namespace gw {
	struct Scenario {
		MapSize mapSize;
		std::pair<double,double> player;
		std::list<std::pair<std::pair<double,double>,double>> obstacles;
	};

	extern Scenario linear;
	extern Scenario absolute;
	extern Scenario square;
}

#endif