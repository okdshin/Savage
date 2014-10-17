#ifndef SAVAGE_LINEAR_MATH_HPP
#define SAVAGE_LINEAR_MATH_HPP
//20141009
#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
namespace savage {
	template<typename T>
	struct pi {
		static T constexpr value = T(3.1415926535897932384626433832795028841971);
	};
}// namespace savage
#endif //SAVAGE_LINEAR_MATH_HPP
