#ifndef SAVAGE_RENDER_PARENT_TRANSFORM_INHERITOR_HPP
#define SAVAGE_RENDER_PARENT_TRANSFORM_INHERITOR_HPP
//20141007
#include <savage/renderer/scene_node.hpp>
namespace savage {
	namespace renderer {
		namespace parent_transform_inheritors {
			template<typename T>
			class parent_transform_inheritor {
			public:
				parent_transform_inheritor(T* t) : t_(*t) {}

				void operator()(savage::renderer::scene_node& self) {
					//savage::renderer::inherit_parent_transform(scene_node);
					self.translation(self.parent()->translation());
					self.rotation(self.parent()->rotation());
					self.scale(self.parent()->scale());
					t_(self);
				}

			private:
				T& t_;
			};
		}// namespace parent_transform_inheritors
		using savage::renderer::parent_transform_inheritors::parent_transform_inheritor;
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDER_PARENT_TRANSFORM_INHERITOR_HPP
