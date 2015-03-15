#ifndef SAVAGE_PHYSICS_ENGINE_HPP
#define SAVAGE_PHYSICS_ENGINE_HPP
//20141027
#include <btBulletDynamicsCommon.h>
#include <savage/physics/linear_math_conversion.hpp>

namespace savage {
	namespace physics {
		class engine {
		public:
			engine() :
				broadphase_(), collision_config_(),
				dispatcher_(&collision_config_), solver_(),
				dynamics_world_(&dispatcher_, &broadphase_, &solver_, &collision_config_) {}
			~engine() {
				dynamics_world_.removeRigidBody(ground_rigid_body_.get());
				for(auto& rigid_body : rigid_bodies_) {
					dynamics_world_.removeRigidBody(rigid_body.get());
				}
			}

			void init() {
				dynamics_world_.setGravity(btVector3(0, -10, 0));

				//create ground
				ground_shape_ = std::make_unique<btStaticPlaneShape>(btVector3(0, 1, 0), 1);
				ground_motion_state_ = std::make_unique<btDefaultMotionState>(
					btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
				btRigidBody::btRigidBodyConstructionInfo ground_rigid_body_ci(
					0, ground_motion_state_.get(), ground_shape_.get(), btVector3(0, 0, 0));
				ground_rigid_body_ = std::make_unique<btRigidBody>(ground_rigid_body_ci);
				dynamics_world_.addRigidBody(ground_rigid_body_.get());
			}

			btCollisionObject* create_collision_body(
				float mass, 
				btMotionState* motion_state,
				btCollisionShape* shape,
				glm::vec3 const& inertia
			) {
				btRigidBody::btRigidBodyConstructionInfo rigid_body_ci(
					mass, motion_state, shape, 
					savage::physics::glm_to_bt(inertia)
				);

				auto rigid_body = std::make_unique<btRigidBody>(rigid_body_ci);
				dynamics_world_.addRigidBody(rigid_body.get());
				rigid_bodies_.emplace_back(std::move(rigid_body));
				return rigid_bodies_.back().get();
			}

			void update() {
				dynamics_world_.stepSimulation(1 / 60.f);
			}

		private:
			btDbvtBroadphase broadphase_;
			btDefaultCollisionConfiguration collision_config_;
			btCollisionDispatcher dispatcher_;
			btSequentialImpulseConstraintSolver solver_;
			btDiscreteDynamicsWorld dynamics_world_;

			std::unique_ptr<btStaticPlaneShape> ground_shape_;
			std::unique_ptr<btDefaultMotionState> ground_motion_state_;
			std::unique_ptr<btRigidBody> ground_rigid_body_;
			std::vector<std::unique_ptr<btRigidBody>> rigid_bodies_;
			std::vector<std::unique_ptr<btBoxShape>> box_shapes_;
		};
	}// namespace physics
}// namespace savage

#endif //SAVAGE_PHYSICS_ENGINE_HPP
