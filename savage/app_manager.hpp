#ifndef SAVAGE_APP_MANAGER_HPP
#define SAVAGE_APP_MANAGER_HPP
//20140918
#include <memory>
#include <savage/fundamental.hpp>
#include <savage/window.hpp>
namespace savage {
	namespace app_managers {
		class app_holder_base {
		public:
			virtual void update() = 0;
		};
		template<typename Application>
		class app_holder : 
			public savage::app_managers::app_holder_base 
		{
		public:
			typedef Application app_type;
			app_holder(app_type& app) : 
				app_(app) {}

			virtual void update() {
				app_.update();
			}
		private:
			app_type& app_;
		};
		class app_manager {
			app_manager(app_manager const&) = delete;
			app_manager& operator=(app_manager const&) = delete;
		public:
			static app_manager& instance() {
				static app_manager instance;
				return instance;
			}
			template<typename Application>
			void register_app(Application& app) {
				app_holder_.reset(new savage::app_managers::app_holder<Application>(app));
			}

			void update_app() {
				app_holder_->update();
			}

		private:
			app_manager() : app_holder_() {}
			std::unique_ptr<app_holder_base> app_holder_;
		};
	}// namespace app_managers
	using savage::app_managers::app_manager;

	template<typename Application>
	void register_app(Application& app) {
		savage::app_manager::instance().register_app(app);
	}

	void main_loop() {
		savage::window& window = savage::window_manager::instance().window();
		while (!savage::is_window_closed(window))
		{
			savage::app_manager::instance().update_app();
			savage::swap_buffers(window);
			savage::poll_events();
		}
	}

	void initialize(
		std::string const& window_title,
		std::size_t window_width,
		std::size_t window_height,
		bool is_fullscreen
	) {
		savage::opengl_utility_init();
		savage::window_manager::instance().initialize(
			window_title, window_width, window_height, is_fullscreen);
		savage::make_context_current(savage::window_manager::instance().window());
		savage::opengl_init();
	}

	int get_key(int key) {
		return glfwGetKey(savage::window_manager::instance().window().handle(), key);
	}
	glm::vec2 get_cursor_position() {
		double x, y;
		glfwGetCursorPos(savage::window_manager::instance().window().handle(), &x, &y);
		return glm::vec2(x, y);
	}
}// namespace savage

#endif //SAVAGE_APP_MANAGER_HPP
