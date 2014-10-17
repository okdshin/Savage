#ifndef SAVAGE_RENDERER_TRANSFORMS_GIVER_PTR_HPP
#define SAVAGE_RENDERER_TRANSFORMS_GIVER_PTR_HPP
//20141009
namespace savage {
	namespace renderer {
		namespace transforms_giver_ptrs { class holder_base {
			public:
				virtual ~holder_base() {}
				virtual holder_base* clone() const = 0;
				virtual glm::mat4 translation() const = 0;
				virtual glm::mat4 rotation() const = 0;
				virtual glm::mat4 scale() const = 0;
			};
			template<typename T>
			class holder : public holder_base {
			public:
				explicit holder(T const& t) : t_(t) {}
				/*virtual*/ holder_base* clone() const { return new holder(t_); }
				/*virtual*/ glm::mat4 translation() const { return t_.translation(); }
				/*virtual*/ glm::mat4 rotation() const { return t_.rotation(); }
				/*virtual*/ glm::mat4 scale() const { return t_.scale(); }
			private:
				T const& t_;
			};
			class transforms_giver_ptr {
			public:
				template<typename T>
				transforms_giver_ptr(T const*const t) : 
					holder_(new savage::renderer::transforms_giver_ptrs::holder<T>(*t)) {}
				transforms_giver_ptr(transforms_giver_ptr const& other) : 
					holder_(other.holder_->clone()) {}
				transforms_giver_ptr& operator=(transforms_giver_ptr const& other) {
					delete holder_;
					holder_ = other.holder_->clone();
					return *this;
				}
				~transforms_giver_ptr() {
					delete holder_;
				}
				glm::mat4 translation() const { return holder_->translation(); }
				glm::mat4 rotation() const { return holder_->rotation(); }
				glm::mat4 scale() const { return holder_->scale(); }
			private:
				savage::renderer::transforms_giver_ptrs::holder_base const* holder_;
			};
		}// namespace transforms_giver_ptrs
		using savage::renderer::transforms_giver_ptrs::transforms_giver_ptr;
		glm::vec3 position(transforms_giver_ptr const& transforms) {
			return glm::vec3(transforms.translation()*glm::vec4(0.f, 0.f, 0.f, 1.f));
		}
		glm::vec3 direction(transforms_giver_ptr const& transforms) {
			return glm::vec3(transforms.rotation()*glm::vec4(0.f, 0.f, -1.f, 0.f));
		}
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDERER_TRANSFORMS_GIVER_PTR_HPP
