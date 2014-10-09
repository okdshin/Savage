#ifndef SAVAGE_RENDERER_RENDERER_HPP
#define SAVAGE_RENDERER_RENDERER_HPP
//20140917
#include <savage/shader.hpp>
namespace savage {
	namespace renderer {
		class scene_manager {
		public:
			scene_manager() : 
				program_(), 
				model_matrix_uniform_("ModelMatrix"), 
				normal_matrix_uniform_("NormalMatrix"),
				position_attribute_(0, "VertexPosition"),
				color_attribute_(1, "VertexColor"),
				normal_attribute_(2, "VertexNormal"),
				texcoord_attribute_(3, "VertexTexCoord") {}

			void init_shader(
				boost::filesystem::path const& vertex_shader_filepath, 
				boost::filesystem::path const& frag_shader_filepath
			) {
				savage::shader::attach<GL_VERTEX_SHADER>(program_, vertex_shader_filepath);
				savage::shader::attach<GL_FRAGMENT_SHADER>(program_, frag_shader_filepath);
				savage::shader::bind_attribute(program_, position_attribute_);
				savage::shader::bind_attribute(program_, color_attribute_);
				savage::shader::bind_attribute(program_, normal_attribute_);
				savage::shader::bind_attribute(program_, texcoord_attribute_);
				savage::shader::link(program_);
				savage::shader::use(program_);
			}

			savage::renderer::scene_node* 
			add_scene_node(
				savage::shader::entity const& entity, 
				glm::mat4 const& model_matrix
			) {
				scene_nodes_.emplace_back(
					std::make_unique<savage::renderer::scene_node>(
						entity, 
						model_matrix
					)
				);
				return scene_nodes_.back().get();
			}

			void render() const {
				for(auto& scene_node : scene_nodes_) {
					savage::shader::set_uniform(program_, 
						model_matrix_uniform_, 
						scene_node->model_matrix()
					);
					savage::shader::set_uniform(program_, 
						normal_matrix_uniform_,
						glm::transpose(glm::inverse(view_mat*scene_node->model_matrix()))
					);
					scene_node.render();
				}
			}
		private:
			savage::shader::program program_;
			savage::shader::uniform model_matrix_uniform_;
			savage::shader::uniform normal_matrix_uniform_;
			savage::shader::attribute position_attribute_(0, "VertexPosition");
			savage::shader::attribute color_attribute_(1, "VertexColor");
			savage::shader::attribute normal_attribute_(2, "VertexNormal");
			savage::shader::attribute texcoord_attribute_(3, "VertexTexCoord");

		};
	}// namespace renderers
	using savage::renderers::renderer;
}// namespace savage

#endif //SAVAGE_RENDERER_RENDERER_HPP
