#ifndef SAVAGE_RENDER_SCENE_NODE_HPP
#define SAVAGE_RENDER_SCENE_NODE_HPP
//20141007
#include <memory>
#include <vector>
#include <savage/opengl.hpp>
#include <savage/linear_math.hpp>
#include <savage/renderer/projectable_unique_ptr.hpp>
#include <savage/renderer/transforms_giver_ptr.hpp>
namespace savage {
	namespace renderer {
		namespace scene_nodes {
			template<typename Projector>
			class scene_node {
			public:
				template<typename... Args>
				scene_node(
					savage::renderer::transforms_giver_ptr parent,
					Args const&... args
				) : 
					parent_(parent),
					projector_(args...),
					children_(),
					translation_(parent_.translation()),
					rotation_(parent_.rotation()),
					scale_(parent_.scale()) {}

				void project() {
					//projector needs to be able to call below
					projector_(this);
					for(auto& child : children_) {
						child.project();
					}
				}

				//TODO return child pointer
				void add_child(projectable_unique_ptr&& child) {
					children_.emplace_back(std::move(child));
				}

				template<typename T, typename... Args>
				T* create_child(Args const&... args) {
					auto t = std::make_unique<T>(this, args...);
					T* tp = t.get();
					children_.emplace_back(std::move(t));
					return tp;
				}

				scene_node* scale_local(glm::vec3 const& v) {
					local_scale(
						glm::scale(
							local_scale(), 
							glm::vec3(local_rotation()*glm::vec4(v, 0.0))
						)
					);
					return this;
				}
				scene_node* scale_local(float s) {
					return scale_local(glm::vec3(s,s,s));
				}
				scene_node* rotate_local(float roll, float pitch, float yaw) {
					local_rotation(
						glm::rotate(
							glm::rotate(
								glm::rotate(
									local_rotation(), 
									roll,
									glm::vec3(0.f, 0.f, 1.f)
								), 
								pitch,
								glm::vec3(0.f, 1.f, 0.f)
							), 
							yaw,
							glm::vec3(1.f, 0.f, 0.f)
						)
					);
					return this;	
				}
				scene_node* translate_local(glm::vec3 const& v) {
					/*
					translation_ = parent_.translation()
						*local_translation()
						*rotation()
						*glm::translate(glm::mat4(), v)
						;
					*/
					local_translation(
						glm::translate(local_translation(),
							glm::vec3(local_rotation()*glm::vec4(v, 0.0))
						)
					);

					return this;
				}
				scene_node* translate_local(float x, float y, float z) {
					return translate_local(glm::vec3(x, y, z));
				}

				scene_node* scale(float x, float y, float z) {
					scale_ = glm::scale(scale_, glm::vec3(x, y, z));
					return this;
				}
				scene_node* scale(float s) {
					return scale(s,s,s);
				}
				scene_node* rotate(float roll, float pitch, float yaw) {
					rotation_ = 
						glm::rotate(
							glm::rotate(
								glm::rotate(
									rotation_, 
									roll,
									glm::vec3(0.f, 0.f, 1.f)
								), 
								pitch,
								glm::vec3(0.f, 1.f, 0.f)
							), 
							yaw,
							glm::vec3(1.f, 0.f, 0.f)
						);
					return this;	
				}
				scene_node* translate(glm::vec3 const& v) {
					translation_ = glm::translate(translation_, v);
					return this;
				}
				scene_node* translate(float x, float y, float z) {
					return translate(glm::vec3(x, y, z));
				}

				glm::mat4 local_translation() const {
					return glm::inverse(parent_.translation())*translation_; 
				}
				glm::mat4 local_rotation() const {
					return glm::inverse(parent_.rotation())*rotation_;
				}
				glm::mat4 local_scale() const {
					return glm::inverse(parent_.scale())*scale_;
				}
				scene_node* local_translation(glm::mat4 const& ltran) {
					translation(parent().translation()*ltran);
					return this;
				}
				scene_node* local_rotation(glm::mat4 const& lrot) {
					rotation(parent().rotation()*lrot);
					return this;
				}
				scene_node* local_scale(glm::mat4 const& lsca) {
					scale(parent().scale()*lsca);
					return this;
				}
				glm::mat4 translation() const { return translation_; }
				glm::mat4 rotation() const { return rotation_; }
				glm::mat4 scale() const { return scale_; }
				void translation(glm::mat4 const& tran) { translation_ = tran; }
				void rotation(glm::mat4 const& rot) { rotation_ = rot; }
				void scale(glm::mat4 const& sca) { scale_ = sca; }

				savage::renderer::transforms_giver_ptr parent() { return parent_; }

				Projector& projector() { return projector_; }
				Projector const& projector() const { return projector_; }

			private:
				savage::renderer::transforms_giver_ptr parent_;
				Projector projector_;
				std::vector<projectable_unique_ptr> children_;
				glm::mat4 translation_;
				glm::mat4 rotation_;
				glm::mat4 scale_;
			};
		}// namespace scene_nodes
		using savage::renderer::scene_nodes::scene_node;

		/*
		template<typename T>
		void move(
			savage::renderer::scene_node<T>& scene_node, 
			float length, float angle
		) {
			glm::vec3 const dir = glm::rotate(
				scene_node.direction(), 
				angle, 
				scene_node.upper_vector()
			);
			scene_node.translate(length*dir);
		}
		template<typename T>
		void turn(
			savage::renderer::scene_node<T>& scene_node, 
			float pitch, float yaw, glm::vec3 const& normal_vector
		) {
			scene_node.rotate(yaw, normal_vector);
			glm::vec3 const axis = glm::rotateY(
				scene_node.direction(), 
				savage::pi<float>::value/2.0f
			);
			scene_node.rotate(pitch, axis);
		}
		
		template<typename T>
		void move_on_plane(
			savage::renderer::scene_node<T>& scene_node, 
			float length, float angle
		) {
			glm::vec3 direction = scene_node.direction();
			glm::vec3 direction_on_xz = direction; direction_on_xz[1] = 0.f;
			glm::vec3 move_direction = glm::normalize(
					glm::rotate(direction_on_xz, angle, glm::vec3(0.f, 1.f, 0.f)));
			scene_node.translate(length*move_direction);
		}
		template<typename T>
		void turn_on_plane(
			savage::renderer::scene_node<T>& scene_node, 
			float pitch, float yaw
		) {
			glm::vec3 direction = scene_node.direction();
			glm::vec3 const axis = glm::rotateY(
				glm::vec3(direction[0], 0.f, direction[2]), 
				savage::pi<float>::value/2.0f
			);
			scene_node.rotate(pitch, axis);
			glm::vec3 yaw_axis = scene_node.position(); yaw_axis[1] = 1.f;
			scene_node.rotate(yaw, yaw_axis);
		}
		*/
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDER_SCENE_NODE_HPP
