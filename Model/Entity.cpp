#include "Entity.h"

namespace gw {
	Entity::Entity(std::pair<float, float> position, float radius):
		m_position(position),
		m_radius(radius)
	{
		m_radius_squared = radius * radius;
	}

	bool Entity::IsHitBy(std::pair<float, float> coords) const {
		const float DifferenceX = coords.first - m_position.first;
		const float DifferenceY = coords.second - m_position.second;

		return (DifferenceX * DifferenceX + DifferenceY * DifferenceY) <= m_radius_squared;
	}
}