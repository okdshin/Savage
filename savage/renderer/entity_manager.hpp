#ifndef SAVAGE_RENDERER_ENTITY_MANAGER_HPP
#define SAVAGE_RENDERER_ENTITY_MANAGER_HPP
//20140917
#include <vector>
#include <boost/filesystem.hpp>
#include <savage/opengl.hpp>
#include <savage/shader.hpp>
#include <savage/renderer/entity.hpp>
namespace savage {
	namespace renderer {
		namespace entity_managers {
			class entity_manager {
			public:
				entity_manager(
					savage::shader::program const& program,
					savage::renderer::entity_info const& entity_info
				) : 
					program_(program), entity_info_(entity_info) {}

				void bind_attribute() const {
					savage::shader::bind_attribute(
						program_, entity_info_.position_attribute());
					savage::shader::bind_attribute(
						program_, entity_info_.normal_attribute());
					savage::shader::bind_attribute(
						program_, entity_info_.texcoord_attribute());
				}

				savage::renderer::entity*
				create_entity() {
					entities_.emplace_back(std::make_unique<savage::renderer::entity>(
						program_, entity_info_
					));
					return entities_.back().get();
				}

			private:
				savage::shader::program const& program_;
				savage::renderer::entity_info entity_info_;
				std::vector<std::unique_ptr<savage::renderer::entity>> entities_;
			};
		}// namespace entity_managers
		using savage::renderer::entity_managers::entity_manager;
		struct entity_load_error : public std::exception {
			entity_load_error(std::string const& message) : message_(message) {}
			virtual char const* what() const throw() {
				return ("entity loading failed. \"" + message_ + "\"").c_str();
			}
		private:
			std::string message_;
		};
		std::vector<savage::renderer::entity*>
		load_obj(
			savage::renderer::entity_manager& entity_manager,
			boost::filesystem::path const& filename
		) {
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;
			std::string err = 
				tinyobj::LoadObj(
					shapes, 
					materials, 
					filename.c_str(), 
					filename.parent_path().c_str()
				);
			if(!err.empty()) {
				throw savage::renderer::entity_load_error(err);
			}
			std::vector<savage::renderer::entity*> entities;
			for(std::size_t i = 0; i < shapes.size(); ++i) {
				auto const& shape = shapes[i];
				std::vector<glm::vec3> position_data, normal_data;
				std::vector<glm::vec2> texcoord_data;
				/*
				if(shape.mesh.normals.empty()) {
					throw savage::renderer::entity_load_error("no normal");
				}
				if(shape.mesh.texcoords.empty()) {
					throw savage::renderer::entity_load_error("no texcoord");
				}
				*/
				/*
				for(std::size_t index : shape.mesh.indices) {
					position_data.push_back(glm::vec3(
				}
				*/
				for(std::size_t j = 0; j < shape.mesh.indices.size(); ++j) {
					std::size_t index = shape.mesh.indices[j];
					position_data.push_back(glm::vec3(
						shape.mesh.positions[3*index],
						shape.mesh.positions[3*index+1],
						shape.mesh.positions[3*index+2]
					));
					if(!shape.mesh.normals.empty()) {
						normal_data.push_back(glm::vec3(
							shape.mesh.normals[3*index],
							shape.mesh.normals[3*index+1],
							shape.mesh.normals[3*index+2]
						));
					}
					if(!shape.mesh.texcoords.empty()) {
						texcoord_data.push_back(glm::vec2(
							shape.mesh.texcoords[2*index],
							shape.mesh.texcoords[2*index+1]
						));
					}
				}
				savage::shader::image kd_image;
				if(shape.mesh.material_ids.front() != -1) {
					unsigned int const material_index = shape.mesh.material_ids.at(0);
					std::string diffuse_texname = materials[material_index].diffuse_texname;
					std::cout << "\"" << diffuse_texname << "\"" << std::endl;
					//throw savage::renderer::entity_load_error("no material");
					kd_image = 
						savage::shader::load_png(filename.parent_path()/diffuse_texname);
				}
				else {
					kd_image = savage::shader::load_png("tree/bark_loo.png");
				}

				//(filename.parent_path()/material.diffuse_texname).c_str());
				savage::renderer::entity* entity = entity_manager.create_entity();
				entity->init(position_data, normal_data, texcoord_data, kd_image);
				entities.push_back(entity);
			}
			return entities;
		}
	}// namespace renderer
}// namespace savage
#endif //SAVAGE_RENDERER_ENTITY_MANAGER_HPP
