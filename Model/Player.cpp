#include "Player.h"

namespace gw {
	Player::Player() {}

	void Player::AddCharater(std::pair<double,double> position) {
		m_charachters.push_back(Entity(position, PLAYER_RADIUS));
	}
}
