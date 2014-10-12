#ifndef SAVAGE_RENDERER_SCENE_ROOT_HPP
#define SAVAGE_RENDERER_SCENE_ROOT_HPP
//20140917
#include <vector>
#include <boost/filesystem.hpp>
#include <savage/opengl.hpp>
#include <savage/shader.hpp>
#include <savage/renderer/scene_node.hpp>
namespace savage {
	namespace renderer {
		struct none_projector {
			template<typename T>
			glm::mat4 operator()(savage::renderer::scene_node<T> const*const) {
				return glm::mat4();
			}
		};
		namespace scene_roots {
			class scene_root {
			public:
				scene_root() : root_(this){}
				savage::renderer::scene_node<none_projector>& operator()() { return root_; }
				glm::mat4 translation() const { return glm::mat4(); }
				glm::mat4 rotation() const { return glm::mat4(); }
				glm::mat4 scale() const { return glm::mat4(); }

			private:
				savage::renderer::scene_node<savage::renderer::none_projector> root_;
			};
		}// namespace scene_roots
		using savage::renderer::scene_roots::scene_root;
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDERER_SCENE_ROOT_HPP
