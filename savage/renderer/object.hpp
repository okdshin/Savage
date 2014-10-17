#ifndef SAVAGE_RENDER_OBJECT_HPP
#define SAVAGE_RENDER_OBJECT_HPP
//20141007
#include <memory>
#include <vector>
#include <savage/opengl.hpp>
namespace savage {
	namespace renderer {
		namespace objects {
			class object {
			public:
				object(savage::shader::program const& program,
					savage::shader::uniform const& model_matrix_uniform,
					savage::renderer::entity const& entity
				) : 
					program_(program), 
					model_matrix_uniform_(model_matrix_uniform), 
					entity_(entity) {}

				void operator()(savage::renderer::scene_node& scene_node) const {
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
			};
		}// namespace objects
		using savage::renderer::objects::object;
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDER_OBJECT_HPP
