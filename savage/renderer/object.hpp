#ifndef SAVAGE_RENDER_OBJECT_HPP
#define SAVAGE_RENDER_OBJECT_HPP
//20141007
#include <memory>
#include <vector>
#include <savage/opengl.hpp>
namespace savage {
	namespace renderer {
		class object_projector {
		public:
			object_projector(savage::shader::program const& program,
				savage::shader::uniform const& model_matrix_uniform,
				savage::renderer::entity const& entity
			) : 
				program_(program), 
				model_matrix_uniform_(model_matrix_uniform), 
				entity_(entity) {}

			template<typename T>
			void operator()(savage::renderer::scene_node<T>* scene_node) const {
				set_uniform(program_, model_matrix_uniform_, 
					scene_node->translation()*scene_node->rotation()*scene_node->scale());
				entity_.render();
			}

		private:
			savage::shader::program const& program_;
			savage::shader::uniform model_matrix_uniform_;
			savage::renderer::entity const& entity_;
		};
		using object = savage::renderer::scene_node<object_projector>;
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDER_OBJECT_HPP
