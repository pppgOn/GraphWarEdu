#ifndef _Entity_
#define _Entity_

#include <utility>

namespace gw {
	class Entity{
		public:
			Entity(std::pair<float, float> position, float radius);
			bool IsHitBy(std::pair<float, float> coords) const;

			std::pair<float, float> m_position;
			float m_radius;
		
		private:
			float m_radius_squared;
	};
}

#endif
