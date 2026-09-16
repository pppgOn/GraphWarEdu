#include "Character.h"
#include <iostream>

namespace gw {
	Character::Character(std::pair<float, float> position):
		Entity(position, CHARACTER_RADIUS),
		m_currentFunction("x")
	{
	}
		
	void Character::setCurrentFunction(std::string function) {
		m_currentFunction = Function(function);
	}

	std::string Character::getCurrentFunction() const {
		return m_currentFunction.toString();
	}

	float Character::evaluateFonction(float x) const {
		return m_currentFunction.evaluate(x);
	}

}