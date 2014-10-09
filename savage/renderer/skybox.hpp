#ifndef SAVAGE_RENDERER_SKYBOX_HPP
#define SAVAGE_RENDERER_SKYBOX_HPP
//20140917
#include <vector>
#include <savage/shader.hpp>
#include <savage/renderer/entity.hpp>
namespace savage {
	namespace renderer {
		namespace skyboxs {
			/*
			class skybox {
				skybox(skybox const&) = delete;
				skybox& operator=(skybox const&) = delete;
			public:
				skybox(
					savage::shader::program& program, 
					savage::shader::uniform const& model_matrix_uniform,
					savage::shader::uniform const& normal_matrix_uniform,
					savage::shader::attribute const& position_attribute,
					savage::shader::attribute const& color_attribute,
					savage::shader::attribute const& normal_attribute,
					savage::shader::attribute const& texcoord_attribute
				) : program_(program), 
					model_matrix_uniform_(model_matrix_uniform), 
					normal_matrix_uniform_(normal_matrix_uniform), 
					entity_(
						position_attribute, 
						color_attribute, 
						normal_attribute,
						texcoord_attribute
					),
					model_matrix_() 
				{
					entity_.load("cube.obj");
					
				}

				void scale(float s) {
					scale(s,s,s);
				}
				void scale(float x, float y, float z) {
					model_matrix_ = glm::scale(model_matrix_, glm::vec3(x, y, z));
				}
				void position(glm::vec3 const& camera_position) {
					model_matrix_ = glm::translate(glm::mat4(), camera_position);
				}
				void render(glm::mat4 const& view_matrix) const {
					savage::shader::set_uniform(
						program_, model_matrix_uniform_, model_matrix_);

					glm::mat4 normal_matrix = 
						glm::transpose(glm::inverse(view_matrix*model_matrix_));
					savage::shader::set_uniform(
						program_, normal_matrix_uniform_, normal_matrix);

					entity_.render();
				}
			private:
				savage::shader::program& program_;
				savage::shader::uniform model_matrix_uniform_;
				savage::shader::uniform normal_matrix_uniform_;
				savage::renderer::entity entity_;
				glm::mat4 model_matrix_;
			};
			*/
		}// namespace skyboxs
		//using savage::renderer::skyboxs::skybox;
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDERER_SKYBOX_HPP
