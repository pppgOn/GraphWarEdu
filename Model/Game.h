#ifndef _Game_
#define _Game_

#include <string>
#include "Map.h"
#include "Scenario.h"

namespace gw {
	class Game{
		public:
			// Random obstacle and players position for duel
			Game(int playerOneCharactersNumber, int playerTwoCharactersNumber, double mapWitdth, double mapHeight);
			
			// Scenario
			Game(Scenario screnarioName);

			Map m_map;
	};
}

#endif
