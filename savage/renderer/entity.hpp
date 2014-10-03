#ifndef SAVAGE_RENDERER_ENTITY_HPP
#define SAVAGE_RENDERER_ENTITY_HPP
//20140917
#include <savage/opengl.hpp>
#include <savage/3rdparty/tinyobjloader/tiny_obj_loader.h>
#include <savage/3rdparty/tinyobjloader/tiny_obj_loader.cc>
namespace savage {
	namespace renderer { 
		struct entity_load_error : public std::exception {
			entity_load_error(std::string const& message) : message_(message) {}
			virtual char const* what() const throw() {
				return ("entity loading failed. \"" + message_ + "\"").c_str();
			}
		private:
			std::string message_;
		};
		namespace entitys {
			class entity {
			public:
				explicit entity(
					savage::shader::attribute const& position_attribute,
					savage::shader::attribute const& color_attribute,
					savage::shader::attribute const& normal_attribute,
					savage::shader::attribute const& texcoord_attribute
				) : 
					count_(0),
					position_attribute_(position_attribute), 
					color_attribute_(color_attribute), 
					normal_attribute_(normal_attribute), 
					texcoord_attribute_(texcoord_attribute), 
					vertex_array_(), 
					buffer_pool_() {}

				void load(boost::filesystem::path const& filename) {
					std::vector<tinyobj::shape_t> shapes;
					std::vector<tinyobj::material_t> materials;
					char const* parent = "";
					std::string err = 
						tinyobj::LoadObj(shapes, materials, filename.c_str(), parent);
					if(!err.empty()) {
						throw savage::renderer::entity_load_error(err);
					}
					std::vector<glm::vec3> position_data, normal_data, texcoord_data;
					for(std::size_t i = 0; i < shapes.size(); ++i){
						for(std::size_t j = 0; j < shapes[i].mesh.indices.size(); ++j){
							std::size_t index = shapes[i].mesh.indices[j];
							position_data.push_back(glm::vec3(
								shapes[i].mesh.positions[3*index],
								shapes[i].mesh.positions[3*index+1],
								shapes[i].mesh.positions[3*index+2]
							));
							if(!shapes[i].mesh.normals.empty()) {
								normal_data.push_back(glm::vec3(
									shapes[i].mesh.normals[3*index],
									shapes[i].mesh.normals[3*index+1],
									shapes[i].mesh.normals[3*index+2]
								));
							}
							if(!shapes[i].mesh.texcoords.empty()) {
								texcoord_data.push_back(glm::vec3(
									shapes[i].mesh.texcoords[3*index],
									shapes[i].mesh.texcoords[3*index+1],
									shapes[i].mesh.texcoords[3*index+2]
								));
							}
						}
					}
					count_ = position_data.size();
					set_vertex_buffer(position_attribute_, position_data);
					//set_vertex_buffer(color_attribute_, color_data);
					set_vertex_buffer(normal_attribute_, normal_data);
					set_vertex_buffer(texcoord_attribute_, texcoord_data);
				}
				
				void render() const {
					savage::shader::bind(vertex_array_);
					glDrawArrays(GL_TRIANGLES, 0, count_);
				}

			private:
				template<typename Data>
				void set_vertex_buffer(
					savage::shader::attribute const& attribute,
					Data const& data
				) {
					savage::shader::bind(vertex_array_);
					auto vertex_buffer = std::make_unique<savage::shader::vertex_buffer>();
					savage::shader::set_data<GL_ARRAY_BUFFER, GL_STATIC_DRAW>(
						*vertex_buffer, attribute, data);
					savage::shader::enable_vertex_attribute(attribute);
					buffer_pool_.emplace_back(std::move(vertex_buffer));
				}
				
				std::size_t count_;
				savage::shader::attribute 
					position_attribute_, 
					color_attribute_, 
					normal_attribute_,
					texcoord_attribute_;
				savage::shader::vertex_array vertex_array_;
				std::vector<std::unique_ptr<savage::shader::vertex_buffer> > buffer_pool_;
			};
		}// namespace entitys
		using savage::renderer::entitys::entity;
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDERER_ENTITY_HPP
