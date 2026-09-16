#include "Player.h"

namespace gw {
	Player::Player() {}

	void Player::AddCharacter(std::pair<float,float> position) {
		m_charachters.push_back(Character(position));
	}
}
