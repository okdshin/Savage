#ifndef SAVAGE_RENDERER_ENTITY_MANAGER_HPP
#define SAVAGE_RENDERER_ENTITY_MANAGER_HPP
//20140917
#include <vector>
#include <boost/filesystem.hpp>
#include <savage/opengl.hpp>
#include <savage/shader.hpp>
#include <savage/renderer/scene_node.hpp>
namespace savage {
	namespace renderer {
		namespace entity_managers {
			class entity_manager {
			public:
				entity_manager(savage::shader::program const& program) : 
					program_(program),
					position_attribute_(0, "vec3", "VertexPosition"),
					color_attribute_(1, "vec3", "VertexColor"),
					normal_attribute_(2, "vec3", "VertexNormal"),
					texcoord_attribute_(3, "vec3", "VertexTexCoord") {}

				void bind_attribute(savage::shader::program const& program) const {
					savage::shader::bind_attribute(program, position_attribute_);
					savage::shader::bind_attribute(program, color_attribute_);
					savage::shader::bind_attribute(program, normal_attribute_);
					savage::shader::bind_attribute(program, texcoord_attribute_);
				}

				savage::renderer::entity*
				create_entity() {
					entities_.emplace_back(
						std::make_unique<savage::renderer::entity>(
							program_,
							position_attribute_, 
							color_attribute_, 
							normal_attribute_,
							texcoord_attribute_
						)
					);
					return entities_.back().get();
				}

			private:
				savage::shader::program const& program_;
				savage::shader::attribute position_attribute_;
				savage::shader::attribute color_attribute_;
				savage::shader::attribute normal_attribute_;
				savage::shader::attribute texcoord_attribute_;
				std::vector<std::unique_ptr<savage::renderer::entity>> entities_;
			};
		}// namespace entity_managers
		using savage::renderer::entity_managers::entity_manager;
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDERER_ENTITY_MANAGER_HPP
