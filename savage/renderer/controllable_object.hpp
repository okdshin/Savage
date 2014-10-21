#ifndef SAVAGE_RENDER_CONTROLLABLE_OBJECT_HPP
#define SAVAGE_RENDER_CONTROLLABLE_OBJECT_HPP
//20141007
#include <memory>
#include <vector>
#include <savage/opengl.hpp>
namespace savage {
	namespace renderer {
		class controllable_object_projector {
		public:
			controllable_object_projector(savage::shader::program const& program,
				savage::shader::uniform const& model_matrix_uniform,
				savage::renderer::entity const& entity,
				savage::clock const& clock
			) : 
				program_(program), 
				model_matrix_uniform_(model_matrix_uniform), 
				entity_(entity),
				clock_(clock) {}

			template<typename T>
			void operator()(savage::renderer::scene_node<T>* scene_node) const {
				//TODO
				float speed_ = 1;
				const float t = speed_ * savage::delta_second(clock_);
				//scene_node->rotate(t, t, t);
				scene_node->rotate(0, t, 0);
				scene_node->translate_local(t*
				set_uniform(program_, model_matrix_uniform_, 
					scene_node->translation()*scene_node->rotation()*scene_node->scale());
				entity_.render();
			}

		private:
			savage::shader::program const& program_;
			savage::shader::uniform model_matrix_uniform_;
			savage::renderer::entity const& entity_;
			savage::clock const& clock_;
		};
		using controllable_object = savage::renderer::scene_node<controllable_object_projector>;
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDER_CONTROLLABLE_OBJECT_HPP
