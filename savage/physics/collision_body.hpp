#ifndef SAVAGE_PHYSICS_COLLISION_BODY_HPP
#define SAVAGE_PHYSICS_COLLISION_BODY_HPP
//20141027
#include <btBulletDynamicsCommon.h>
#include <savage/physics/linear_math_conversion.hpp>
namespace savage {
	namespace physics {
		class collision_body {
		public:
			collision_body(savage::physics::engine& engine) : 
				engine_(engine), motion_state_() {}

			template<typename MotionState>
			void init(btCollisionShape* shape,
				float mass, glm::vec3 const& inertia,
				MotionState const& motion_state
			) {
				motion_state_ = std::make_shared<MotionState>(motion_state);
				collision_body_ = engine_.create_collision_body(
					mass, 
					motion_state_.get(),
					shape, 
					inertia
				);
			}
			void operator()(savage::renderer::scene_node& self) {
				btTransform trans = collision_body_->getWorldTransform();
				savage::renderer::position(
					self, savage::physics::bt_to_glm(trans.getOrigin()));
				self.rotation(
					glm::rotate(
						glm::mat4(), 
						/*(3.14f/180)**/trans.getRotation().getAngle(),
						savage::physics::bt_to_glm(trans.getRotation().getAxis())
					)
				);
			}
		private:
			savage::physics::engine& engine_;
			btCollisionObject* collision_body_;
			std::shared_ptr<btMotionState> motion_state_;
		};
	}// namespace physics
}// namespace savage

#endif //SAVAGE_PHYSICS_COLLISION_BODY_HPP
