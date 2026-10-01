#ifndef _Character_
#define _Character_

#include <utility>
#include "Entity.h"
#include "Function.h"

#define CHARACTER_RADIUS 0.75

namespace gw {
	class Character : public Entity {
		public:
			Character(std::pair<float, float> position);

			void setCurrentFunction(std::string function);
			std::string getCurrentFunction() const;
			float evaluateFonction(float x) const;

			bool operator == (const Character& character) const { return m_position == character.m_position; }
		private:
			Function m_currentFunction;
			float m_delta;
	};
}

#endif
