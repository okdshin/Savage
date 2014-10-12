#ifndef SAVAGE_RENDERER_CAMERAMAN_HPP
#define SAVAGE_RENDERER_CAMERAMAN_HPP
//20141003
#include <savage/opengl.hpp>
#include <savage/fundamental.hpp>
#include <savage/shader.hpp>
namespace savage {
	namespace renderer {
/*
		namespace cameramans {
			class cameraman {
			public:
				cameraman(
					savage::shader::program const& program, 
					float angle,
					float aspect,
					float near,
					float far,
					glm::vec3 const& position, 
					glm::vec3 const& direction,
					glm::vec3 const& upper,
					float move_speed
				) : camera_(program, angle, aspect, near, far, position, direction, upper),
				move_speed_(move_speed)
				{}
				void update_projection() const {
					camera_.update_projection();
				}
				void update_view() const {
					camera_.update_view();
				}
				void move(float delta_time, float angle) {
					glm::vec3 move_direction_temp = 
						glm::rotate(camera_.direction(), angle, camera_.upper());
					move_direction_temp[1] = 0.0f;
					const auto move_direction = glm::normalize(move_direction_temp);
					camera_.position(
						camera_.position()+move_speed_*delta_time*move_direction
					);
				}
				void turn(float pitch, float yaw) {
					camera_.direction(
						glm::rotate(camera_.direction(), yaw, glm::vec3(0,1,0))
					);
					auto normal = glm::rotateY(camera_.direction(), savage::pi<float>::value/2.0f);
					camera_.direction(
						glm::rotate(camera_.direction(), pitch, normal)
					);
				}
			private:
				savage::renderer::camera camera_;
				float move_speed_;
			};
		}// namespace cameramans
		using savage::renderer::cameramans::cameraman;
		void keyboard_controll(savage::renderer::cameraman& cameraman, float dt,
			int key, int scancode, int action, int mods
		) {
			if(key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
				cameraman.move(dt, 0); 
			}else
			if(key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
				cameraman.move(dt, savage::pi<float>::value); 
			}else
			if(key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
				cameraman.move(dt, savage::pi<float>::value/2.0f); 
			}else
			if(key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
				cameraman.move(dt, -savage::pi<float>::value/2.0f); 
			}
		}
		*/
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDERER_CAMERAMAN_HPP
