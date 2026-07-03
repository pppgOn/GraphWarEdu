#include "Entity.h"

namespace gw {
	Entity::Entity(std::pair<double, double> position, double radius):
		m_position(position),
		m_radius(radius)
	{
		m_radius_squared = radius * radius;
	}

	bool Entity::IsHitBy(std::pair<double, double> coords) const {
		const double DifferenceX = coords.first - m_position.first;
		const double DifferenceY = coords.second - m_position.second;

		return (DifferenceX * DifferenceX + DifferenceY * DifferenceY) <= m_radius_squared;
	}
}