#ifndef SAVAGE_RENDERER_ENTITY_HPP
#define SAVAGE_RENDERER_ENTITY_HPP
//20140917
#include <savage/opengl.hpp>
#include <savage/shader/texture.hpp>
#include <savage/3rdparty/tinyobjloader/tiny_obj_loader.h>
#include <savage/3rdparty/tinyobjloader/tiny_obj_loader.cc>
namespace savage {
	namespace renderer { 
		namespace entitys {
			struct entity_info {
			public:
				entity_info(
					savage::shader::attribute const& position_attribute,
					savage::shader::attribute const& normal_attribute,
					savage::shader::attribute const& texcoord_attribute,
					savage::shader::uniform const& kd_tex_uniform
				) :
					position_attribute_(position_attribute),
					normal_attribute_(normal_attribute),
					texcoord_attribute_(texcoord_attribute),
					kd_tex_uniform_(kd_tex_uniform) {}
				savage::shader::attribute const& position_attribute() const {
					return position_attribute_;
				}
				savage::shader::attribute const& normal_attribute() const {
					return normal_attribute_;
				}
				savage::shader::attribute const& texcoord_attribute() const {
					return texcoord_attribute_;
				}
				savage::shader::uniform const& kd_tex_uniform() const {
					return kd_tex_uniform_;
				}
			private:
				savage::shader::attribute position_attribute_;
				savage::shader::attribute normal_attribute_;
				savage::shader::attribute texcoord_attribute_;
				savage::shader::uniform kd_tex_uniform_;
			};
			class entity {
			public:
				explicit entity(
					savage::shader::program const& program,
					entity_info const& info
				) : 
					program_(program),
					info_(info),
					vertex_array_(), 
					buffer_pool_(),
					kd_texture_(0),
					kd_tex_image_(),
					vertex_count_(0) {}

				void init(
					std::vector<glm::vec3> const& position_data,
					std::vector<glm::vec3> const& normal_data,
					std::vector<glm::vec2> const& texcoord_data,
					savage::shader::image const& kd_tex_image
				) {
					vertex_count_ = position_data.size();
					set_vertex_buffer(info_.position_attribute(), position_data);
					set_vertex_buffer(info_.normal_attribute(), normal_data);
					set_vertex_buffer(info_.texcoord_attribute(), texcoord_data);
					kd_tex_image_ = kd_tex_image;
				}
				
				void render() const {
					savage::shader::set_image<GL_RGB>(kd_texture_, kd_tex_image_);
					savage::shader::set_texture_uniform(
						program_, 
						info_.kd_tex_uniform(),
						kd_texture_
					);
					savage::shader::bind(vertex_array_);
					glDrawArrays(GL_TRIANGLES, 0, vertex_count_);
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
				
				savage::shader::program const& program_;
				savage::renderer::entitys::entity_info info_;
				savage::shader::vertex_array vertex_array_;
				std::vector<std::unique_ptr<savage::shader::vertex_buffer> > buffer_pool_;
				savage::shader::texture kd_texture_;
				savage::shader::image kd_tex_image_;
				unsigned int vertex_count_;
			};
		}// namespace entitys
		using savage::renderer::entitys::entity_info;
		using savage::renderer::entitys::entity;
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDERER_ENTITY_HPP
