#ifndef _Entity_
#define _Entity_

#include <utility>

namespace gw {
	class Entity{
		public:
			Entity(std::pair<double, double> position, double radius);
			bool IsHitBy(std::pair<double, double> coords) const;

			std::pair<double, double> m_position;
			double m_radius;
		
		private:
			double m_radius_squared;
	};
}

#endif
