#ifndef SAVAGE_RENDERER_LIGHT_HPP
#define SAVAGE_RENDERER_LIGHT_HPP
//20141007
#include <savage/shader.hpp>
#include <savage/renderer/scene_node.hpp>
namespace savage {
	namespace renderer {
		namespace lights {
			class point_light_projector {
			public:
				point_light_projector(
					savage::shader::program const& program,
					savage::shader::uniform const& ka_uniform,
					savage::shader::uniform const& kd_uniform,
					savage::shader::uniform const& ks_uniform,
					savage::shader::uniform const& color_uniform,
					savage::shader::uniform const& position_uniform,
					float ka, float kd, float ks, 
					glm::vec3 const& color
				) :
					program_(program),
					ka_uniform_(ka_uniform), 
					kd_uniform_(kd_uniform),
					ks_uniform_(ks_uniform),
					color_uniform_(color_uniform),
					position_uniform_(position_uniform),
					ka_(ka), kd_(kd), ks_(ks),
					color_(color) {}

				template<typename T>
				void operator()(
					savage::renderer::scene_node<T> const*const scene_node
				) const {
					savage::shader::set_uniform(program_, ka_uniform_, ka_);
					savage::shader::set_uniform(program_, kd_uniform_, kd_);
					savage::shader::set_uniform(program_, ks_uniform_, ks_);
					savage::shader::set_uniform(program_, color_uniform_, color_);
					savage::shader::set_uniform(
						program_, 
						position_uniform_,
						savage::renderer::position(scene_node)
					);
				}

			private:
				savage::shader::program const& program_;
				savage::shader::uniform 
					ka_uniform_,
					kd_uniform_,
					ks_uniform_,
					color_uniform_,
					position_uniform_;
				float ka_, kd_, ks_;
				glm::vec3 color_;
			};
		}// namespace lights
		using point_light = savage::renderer::scene_node<
			savage::renderer::lights::point_light_projector
		>;
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDERER_LIGHT_HPP
