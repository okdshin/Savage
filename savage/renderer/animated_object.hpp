#ifndef SAVAGE_RENDER_ANIMATED_OBJECT_HPP
#define SAVAGE_RENDER_ANIMATED_OBJECT_HPP
//20141007
#include <memory>
#include <vector>
#include <savage/opengl.hpp>
#include <savage/renderer/scene_node.hpp>
#include <savage/renderer/scene_node_transform_chain.hpp>
namespace savage {
	namespace renderer {
		namespace animated_objects {
			class animated_object {
			public:
				animated_object(savage::shader::program const& program,
					savage::shader::uniform const& model_matrix_uniform,
					savage::renderer::entity const& entity,
					savage::clock const& clock
				) : 
					program_(program), 
					model_matrix_uniform_(model_matrix_uniform), 
					entity_(entity),
					clock_(clock) {}

				void operator()(savage::renderer::scene_node& scene_node) const {
					float speed_ = 1;
					const float t = speed_ * savage::delta_second(clock_);
					//scene_node->rotate(t, t, t);
					savage::renderer::transform_chain(scene_node).rotate(0, t, 0);
					savage::shader::set_uniform(
						program_, 
						model_matrix_uniform_, 
						savage::renderer::model_matrix(scene_node)
					);
					entity_.render();
				}

			private:
				savage::shader::program const& program_;
				savage::shader::uniform model_matrix_uniform_;
				savage::renderer::entity const& entity_;
				savage::clock const& clock_;
			};
		}// namespace animated_objects
		using savage::renderer::animated_objects::animated_object;
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDER_ANIMATED_OBJECT_HPP
