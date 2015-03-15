#ifndef SAVAGE_PHYSICS_LINEAR_MATH_CONVERSION_HPP
#define SAVAGE_PHYSICS_LINEAR_MATH_CONVERSION_HPP
//20141027
#include <savage/linear_math.hpp>
#include <btBulletDynamicsCommon.h>
namespace savage {
	namespace physics {
		btVector3 glm_to_bt(glm::vec3 const& v) {
			return btVector3(v[0], v[1], v[2]);
		}
		glm::vec3 bt_to_glm(btVector3 const& v) {
			return glm::vec3(v.x(), v.y(), v.z());
		}
	}// namespace physics
}// namespace savage

#endif //SAVAGE_PHYSICS_LINEAR_MATH_CONVERSION_HPP
