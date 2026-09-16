#ifndef _Player_
#define _Player_

#include <list>
#include <string>
#include "Character.h"

namespace gw {
	class Player{
		public:
			Player();
			void AddCharacter(std::pair<float,float> position);

			std::list<Character> m_charachters;
	};
}

#endif
