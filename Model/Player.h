#ifndef _Player_
#define _Player_

#include <list>
#include "Entity.h"

#define PLAYER_RADIUS 0.75

namespace gw {
	class Player{
		public:
			Player();
			void AddCharater(std::pair<float,float> position);

			std::list<Entity> m_charachters;
	};
}

#endif
