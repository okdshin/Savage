#ifndef SAVAGE_RENDER_SCENE_NODE_HPP
#define SAVAGE_RENDER_SCENE_NODE_HPP
//20141007
#include <memory>
#include <vector>
#include <functional>
#include <savage/linear_math.hpp>
namespace savage {
	namespace renderer {
		namespace scene_nodes {
			class scene_node {
			public:
				using projector_type = std::function<void(scene_node&)>;
				scene_node(
					scene_node* parent,
					projector_type projector
				) : 
					parent_(parent),
					projector_(projector),
					children_(),
					translation_(parent_->translation()),
					rotation_(parent_->rotation()),
					scale_(parent_->scale()) {}
				
				scene_node() : 
					parent_(nullptr), 
					projector_([](scene_node&){}),
					children_(),
					translation_(glm::mat4()),
					rotation_(glm::mat4()),
					scale_(glm::mat4()) {}

				scene_node* parent() const { return parent_; }

				void project() {
					projector_(*this);
					for(auto& child : children_) {
						child->project();
					}
				}

				scene_node* create_child(projector_type projector) {
					children_.emplace_back(std::make_unique<scene_node>(this, projector));
					return children_.back().get();
				}

				glm::mat4 translation() const { return translation_; }
				glm::mat4 rotation() const { return rotation_; }
				glm::mat4 scale() const { return scale_; }
				void translation(glm::mat4 const& tran) { translation_ = tran; }
				void rotation(glm::mat4 const& rot) { rotation_ = rot; }
				void scale(glm::mat4 const& sca) { scale_ = sca; }

				glm::mat4 local_translation() const {
					return glm::inverse(parent_->translation())*translation_; 
				}
				glm::mat4 local_rotation() const {
					return glm::inverse(parent_->rotation())*rotation_;
				}
				glm::mat4 local_scale() const {
					return glm::inverse(parent_->scale())*scale_;
				}
				void local_translation(glm::mat4 const& ltran) {
					translation(parent_->translation()*ltran);
				}
				void local_rotation(glm::mat4 const& lrot) {
					rotation(parent_->rotation()*lrot);
				}
				void local_scale(glm::mat4 const& lsca) {
					scale(parent_->scale()*lsca);
				}

			private:
				scene_node* parent_;
				projector_type projector_;
				std::vector<std::unique_ptr<scene_node>> children_;
				glm::mat4 translation_;
				glm::mat4 rotation_;
				glm::mat4 scale_;
			};
		}// namespace scene_nodes
		using savage::renderer::scene_nodes::scene_node;

		void scale_local(scene_node& sn, glm::vec3 const& v) {
			sn.local_scale(
				glm::scale(
					sn.local_scale(), 
					glm::vec3(sn.local_rotation()*glm::vec4(v, 0.0))
				)
			);
		}
		void rotate_local(scene_node& sn, glm::vec3 const& v) {
			sn.local_rotation(
				glm::rotate(
					glm::rotate(
						glm::rotate(
							sn.local_rotation(), 
							v[0],
							glm::vec3(sn.local_rotation()
								*glm::vec4(0.f, 0.f, -1.f, 0.0))
						), 
						v[1],
						glm::vec3(sn.local_rotation()*glm::vec4(0.f, 1.f, 0.f, 0.0))
					), 
					v[2],
					glm::vec3(sn.local_rotation()*glm::vec4(1.f, 0.f, 0.f, 0.0))
				)
			);
		}
		void translate_local(scene_node& sn, glm::vec3 const& v) {
			/*
			translation_ = parent_.translation()
				*local_translation()
				*rotation()
				*glm::translate(glm::mat4(), v)
				;
			*/
			sn.local_translation(
				glm::translate(sn.local_translation(),
					glm::vec3(sn.local_rotation()*glm::vec4(v, 0.0))
				)
			);
		}

		void scale(scene_node& sn, glm::vec3 const& v) {
			sn.scale(glm::scale(sn.scale(), v));
		}
		void rotate(scene_node& sn, glm::vec3 const& v) {
			sn.rotation(
				glm::rotate(
					glm::rotate(
						glm::rotate(
							sn.rotation(), 
							v[0],
							glm::vec3(0.f, 0.f, -1.f)
						), 
						v[1],
						glm::vec3(0.f, 1.f, 0.f)
					), 
					v[2],
					glm::vec3(1.f, 0.f, 0.f)
				)
			);
		}
		void translate(scene_node& sn, glm::vec3 const& v) {
			sn.translation(glm::translate(sn.translation(), v));
		}
		glm::mat4 model_matrix(scene_node const& sn) {
			return sn.translation()*sn.rotation()*sn.scale();
		}
		glm::vec3 position(scene_node const& sn) {
			return glm::vec3(sn.translation()*glm::vec4(0.f, 0.f, 0.f, 1.f));
		}
		glm::vec3 direction(scene_node const& sn) {
			return glm::vec3(sn.rotation()*glm::vec4(0.f, 0.f, -1.f, 0.f));
		}

	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDER_SCENE_NODE_HPP
