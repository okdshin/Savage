#ifndef SAVAGE_RENDERER_CAMERA_HPP
#define SAVAGE_RENDERER_CAMERA_HPP
//20141003
#include <savage/opengl.hpp>
#include <savage/linear_math.hpp>
#include <savage/shader.hpp>
#include <savage/renderer/scene_node.hpp>
namespace savage {
	namespace renderer {
		namespace cameras {
			class camera {
			public:
				camera(
					savage::shader::program const& program, 
					savage::shader::uniform const& projection_matrix_uniform,
					savage::shader::uniform const& view_matrix_uniform,
					float angle,
					float aspect,
					float near,
					float far
				) : 
					program_(program),
					projection_matrix_uniform_(projection_matrix_uniform),
					view_matrix_uniform_(view_matrix_uniform),
					angle_(angle), aspect_(aspect), near_(near), far_(far) {}

				void operator()(
					savage::renderer::scene_node& scene_node
				) const {
					savage::shader::set_uniform(program_, 
						projection_matrix_uniform_, 
						glm::perspective(angle_, aspect_, near_, far_)
					);
					glm::vec3 position = savage::renderer::position(scene_node);
					glm::vec3 direction = savage::renderer::direction(scene_node);
					glm::vec3 direction_on_xz = direction; direction_on_xz[1] = 0.f;
					glm::vec3 axis = glm::rotateY(
							direction_on_xz, -savage::pi<float>::value/2.f);
					glm::vec3 upper = glm::rotate(
							direction, savage::pi<float>::value/2.f, axis);
					savage::shader::set_uniform(
						program_, 
						view_matrix_uniform_,
						glm::lookAt(
							position,
							position+direction,
							upper
						)
					);
				}
			private:
				savage::shader::program const& program_;
				savage::shader::uniform projection_matrix_uniform_, view_matrix_uniform_;
				float angle_, aspect_, near_, far_;
			};
		}// namespace cameras
		using savage::renderer::cameras::camera;
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDERER_CAMERA_HPP
