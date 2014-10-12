#ifndef SAVAGE_RENDERER_PROJECTABLE_UNIQUE_PTR_HPP
#define SAVAGE_RENDERER_PROJECTABLE_UNIQUE_PTR_HPP
//20141009
#include <memory>
namespace savage {
	namespace renderer {
		namespace projectable_unique_ptrs {
			class holder_base {
			public:
				virtual ~holder_base() {}
				virtual void project() = 0;
			};
			template<typename T>
			class holder : public holder_base {
			public:
				explicit holder(std::unique_ptr<T>&& t) : t_(std::move(t)) {}
				/*virtual*/ void project() { t_->project(); }
			private:
				std::unique_ptr<T> t_;
			};
			class projectable_unique_ptr {
			public:
				template<typename T>
				explicit projectable_unique_ptr(std::unique_ptr<T>&& t) : 
					holder_(
						std::make_unique<
							savage::renderer::projectable_unique_ptrs::holder<T>
						>(
							std::move(t)
						)
					) {}
				void project() { holder_->project(); }
			private:
				std::unique_ptr<
					savage::renderer::projectable_unique_ptrs::holder_base
				> holder_;
			};
		}// namespace projectable_unique_ptrs
		using savage::renderer::projectable_unique_ptrs::projectable_unique_ptr;
	}// namespace renderer
}// namespace savage

#endif //SAVAGE_RENDERER_PROJECTABLE_UNIQUE_PTR_HPP
