#ifndef SAVAGE_RENDER_SKYBOX_HPP
#define SAVAGE_RENDER_SKYBOX_HPP
//20141007
#include <memory>
#include <vector>
#include <savage/opengl.hpp>
namespace savage {
	namespace renderer {
		namespace skyboxs {
			class skybox {
			public:
				skybox(savage::shader::program const& program,
					savage::shader::uniform const& model_matrix_uniform,
					savage::renderer::entity const& entity,
					savage::renderer::scene_node const* const camera
				) : 
					program_(program), 
					model_matrix_uniform_(model_matrix_uniform), 
					entity_(entity),
					camera_(camera) {}

				void operator()(savage::renderer::scene_node& self) const {
					self.translation(camera_->translation());
					savage::shader::set_uniform(
						program_, 
						model_matrix_uniform_, 
						savage::renderer::model_matrix(self)
					);
					entity_.render();
				}

			private:
				savage::shader::program const& program_;
				savage::shader::uniform model_matrix_uniform_;
				savage::renderer::entity const& entity_;
				savage::renderer::scene_node const* const camera_;
			};
		}// namespace skyboxs
		using savage::renderer::skyboxs::skybox;
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDER_SKYBOX_HPP
