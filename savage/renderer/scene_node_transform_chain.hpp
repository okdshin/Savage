#ifndef SAVAGE_RENDERER_SCENE_NODE_TRANSFORM_CHAIN_HPP
#define SAVAGE_RENDERER_SCENE_NODE_TRANSFORM_CHAIN_HPP
//20141017

namespace savage {
	namespace renderer {
		namespace scene_node_transform_chains {
			class scene_node_transform_chain {
			public:
				scene_node_transform_chain(savage::renderer::scene_node& scene_node) :
					scene_node_(scene_node) {}

				scene_node_transform_chain& 
				scale_local(glm::vec3 const& v) {
					savage::renderer::scale_local(scene_node_, v);
					return *this;
				}
				scene_node_transform_chain& 
				scale_local(float x, float y, float z) {
					return scale_local(glm::vec3(x, y, z));
				}
				scene_node_transform_chain& 
				scale_local(float s) {
					return scale_local(s, s, s);
				}
				scene_node_transform_chain& 
				rotate_local(glm::vec3 const& v) {
					savage::renderer::rotate_local(scene_node_, v);
					return *this;	
				}
				scene_node_transform_chain& 
				rotate_local(float roll, float pitch, float yaw) {
					return rotate_local(glm::vec3(roll, pitch, yaw));
				}
				scene_node_transform_chain& 
				translate_local(glm::vec3 const& v) {
					savage::renderer::translate_local(scene_node_, v);
					return *this;
				}
				scene_node_transform_chain& 
				translate_local(float x, float y, float z) {
					return translate_local(glm::vec3(x, y, z));
				}

				scene_node_transform_chain& 
				scale(glm::vec3 const& v) {
					savage::renderer::scale(scene_node_, v);
					return *this;
				}
				scene_node_transform_chain& 
				scale(float x, float y, float z) {
					return scale(glm::vec3(x, y, z));
				}
				scene_node_transform_chain& 
				scale(float s) {
					return scale(s, s, s);
				}
				scene_node_transform_chain& 
				rotate(glm::vec3 const& v) {
					savage::renderer::rotate(scene_node_, v);
					return *this;	
				}
				scene_node_transform_chain& 
				rotate(float roll, float pitch, float yaw) {
					savage::renderer::rotate(scene_node_, glm::vec3(roll, pitch, yaw));
					return *this;	
				}
				scene_node_transform_chain& 
				translate(glm::vec3 const& v) {
					savage::renderer::translate(scene_node_, v);
					return *this;
				}
				scene_node_transform_chain& 
				translate(float x, float y, float z) {
					return translate(glm::vec3(x, y, z));
				}
			private:
				savage::renderer::scene_node& scene_node_;
			};
		}// namespace scene_node_transform_chains
		using savage::renderer::scene_node_transform_chains::scene_node_transform_chain;
		savage::renderer::scene_node_transform_chain
		transform_chain(scene_node& scene_node) {
			return savage::renderer::scene_node_transform_chain(scene_node);
		}
	}// renderer
}// namespace savage

#endif //SAVAGE_RENDERER_SCENE_NODE_TRANSFORM_CHAIN_HPP
