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
					translation_(),
					rotation_(),
					scale_() {}
				
				scene_node() : scene_node(nullptr, [](scene_node&){}) {}

				template<typename T>
				typename T::return_type chain(T const& t) {
					return t(*this);
				}

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

				glm::mat4 translation() const {
					return translation_;
				}
				glm::mat4 rotation() const {
					return rotation_;
				}
				glm::mat4 scale() const {
					return scale_;
				}
				void translation(glm::mat4 const& tran) {
					translation_ = tran;
				}
				void rotation(glm::mat4 const& rot) {
					rotation_ = rot;
				}
				void scale(glm::mat4 const& sca) {
					scale_ = sca;
				}

				decltype(auto) children() { return (children_); }
				decltype(auto) children() const { return (children_); }

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

		glm::mat4 local_translation(savage::renderer::scene_node const& sn) {
			return glm::inverse(sn.parent()->translation())*sn.translation();
		}
		glm::mat4 local_rotation(savage::renderer::scene_node const& sn) {
			return glm::inverse(sn.parent()->rotation())*sn.rotation();
		}
		glm::mat4 local_scale(savage::renderer::scene_node const& sn) {
			return glm::inverse(sn.parent()->scale())*sn.scale();
		}
		
		void scale(scene_node& sn, glm::vec3 const& v) {
			sn.scale(glm::scale(sn.scale(), v));
		}
		void scale_local(scene_node& sn, glm::vec3 const& v) {
			savage::renderer::scale(sn, glm::vec3(sn.rotation()*glm::vec4(v, 0.f)));
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
		void rotate_local(scene_node& sn, glm::vec3 const& v) {
			sn.rotation(
				glm::rotate(
					glm::rotate(
						glm::rotate(
							sn.rotation(), 
							v[0],
							glm::vec3(sn.rotation()*glm::vec4(0.f, 0.f, -1.f, 0.0))
						), 
						v[1],
						glm::vec3(sn.rotation()*glm::vec4(0.f, 1.f, 0.f, 0.0))
					), 
					v[2],
					glm::vec3(sn.rotation()*glm::vec4(1.f, 0.f, 0.f, 0.0))
				)
			);
		}

		void translate(scene_node& sn, glm::vec3 const& v) {
			sn.translation(glm::translate(sn.translation(), v));
		}
		void translate_local(scene_node& sn, glm::vec3 const& v) {
			savage::renderer::translate(sn, glm::vec3(sn.rotation()*glm::vec4(v, 0.f)));
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
		void position(scene_node& sn, glm::vec3 const& pos) {
			sn.translation(glm::translate(glm::mat4(), pos));
		}
		/*
		void direction(scene_node& sn, ) {
			sn.rotation(glm::rotation(glm::mat4, ));
		}
		*/

	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDER_SCENE_NODE_HPP
