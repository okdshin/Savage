#ifndef SAVAGE_PHYSICS_SHAPE_MANAGER_HPP
#define SAVAGE_PHYSICS_SHAPE_MANAGER_HPP
//20141027
#include <memory>
#include <btBulletDynamicsCommon.h>
namespace savage {
	namespace physics {
		class shape_manager {
		public:
			shape_manager() : shapes_() {}

			template<typename T, typename ...Args>
			std::tuple<btCollisionShape*, glm::vec3>
			create_shape(float mass, Args const&... args) {
				auto shape = std::make_unique<T>(args...);
				btVector3 inertia;
				shape->calculateLocalInertia(mass, inertia);
				shapes_.emplace_back(std::move(shape));
				return std::make_tuple(shapes_.back().get(), bt_to_glm(inertia));
			}

		private:
			std::vector<std::unique_ptr<btCollisionShape>> shapes_;
		};
	}// namespace physics
}// namespace savage

#endif //SAVAGE_PHYSICS_SHAPE_MANAGER_HPP
