#include "Character.h"
#include <iostream>

namespace gw {
	Character::Character(std::pair<float, float> position):
		Entity(position, CHARACTER_RADIUS),
		m_currentFunction("x"),
		m_delta(0)
	{
	}
		
	void Character::setCurrentFunction(std::string function) {
		m_currentFunction = Function(function);
		m_delta = m_currentFunction.evaluate(m_position.first);
	}

	std::string Character::getCurrentFunction() const {
		return m_currentFunction.toString();
	}

	float Character::evaluateFonction(float x) const {
		return m_currentFunction.evaluate(x) - m_delta;
	}

}