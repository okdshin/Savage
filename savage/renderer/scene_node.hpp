#ifndef SAVAGE_RENDERER_SCENE_NODE_HPP
#define SAVAGE_RENDERER_SCENE_NODE_HPP
//20140917
#include <vector>
#include <savage/opengl.hpp>
#include <savage/renderer/entity.hpp>
namespace savage {
	namespace renderer {
		namespace scene_nodes {
			class scene_node {
				scene_node(scene_node const&) = delete;
				scene_node& operator=(scene_node const&) = delete;
			public:
				scene_node(
					savage::renderer::entity const& entity,
					glm::mat4 const& model_matrix
				) : entity_(entity),
					model_matrix_(model_matrix) {}

				void translate(glm::vec3 const& v) {
					model_matrix_ = glm::translate(model_matrix_, v);
				}
				//void rotate() //TODO
				void scale(float s) {
					scale(s,s,s);
				}
				void scale(float x, float y, float z) {
					model_matrix_ = glm::scale(model_matrix_, glm::vec3(x, y, z));
				}
				glm::mat4 model_matrix() const { return model_matrix_; }
				void render() const {
					/*
					savage::shader::set_uniform(
						program_, model_matrix_uniform_, model_matrix_);

					glm::mat4 normal_matrix = 
						glm::transpose(glm::inverse(view_matrix*model_matrix_));
					savage::shader::set_uniform(
						program_, normal_matrix_uniform_, normal_matrix);
					*/
					entity_.render();
				}
			private:
				savage::renderer::entity const& entity_;
				glm::mat4 model_matrix_;
			};
		}// namespace scene_nodes
		using savage::renderer::scene_nodes::scene_node;
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDERER_SCENE_NODE_HPP
