#ifndef SAVAGE_RENDERER_RENDERER_HPP
#define SAVAGE_RENDERER_RENDERER_HPP
//20140917
#include <savage/shader.hpp>
namespace savage {
	namespace renderers {
		class renderer {
		public:
			void initialize() {
				program_ = savage::shader::create_program();
			}
			template<GLuint shader_type>
			void attach(boost::filesystem::path const& filepath) {
				savage::shader::objects::object object = 
					savage::shader::objects::create_object<type>();

				savage::shader::objects::load_source(object, object_filepath);
				savage::shader::objects::compile(object);
				savage::shader::programs::attach(program_, object);
				shader_object_vect_.emplace_back(object);
			}
			void add_scene_node(
			void render() {
				for(auto& model_object : model_object_vect_) {
					model_object.draw();
				}
			}
			void finalize() {
				for(auto& model_object : model_object_vect_) {
					savage::renderer::model_objects::delete_model(model_object);
				}
				for(auto& shader_object : shader_object_vect_) {
					savage::shader::delete_object(shader_object);	
				}
				savage::shader::delete_program(program_);
			}
		private:
			savage::shader::program program_;
			std::vector<savage::shader::object> shader_object_vect_;
			savage::shader::vertex_array vao_;
		};
	}// namespace renderers
	using savage::renderers::renderer;
}// namespace savage

#endif //SAVAGE_RENDERER_RENDERER_HPP
