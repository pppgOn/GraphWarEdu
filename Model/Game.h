#ifndef _Game_
#define _Game_

#include <string>
#include "Map.h"
#include "Scenario.h"
#include "Character.h"

namespace gw {
	enum State { PlayerTurn, FunctionResolution };

	class Game{
		public:
			// Random obstacle and players position for duel
			Game(int playerOneCharactersNumber, int playerTwoCharactersNumber, float mapWitdth, float mapHeight);
			
			// Scenario
			Game(Scenario screnarioName);

			Character* getCurrentCharacter();

			Map m_map;
			State m_state;
	};
}

#endif
