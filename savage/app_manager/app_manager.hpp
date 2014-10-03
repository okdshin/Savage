#ifndef SAVAGE_APP_MANAGER_APP_MANAGER_HPP
#define SAVAGE_APP_MANAGER_APP_MANAGER_HPP
//20140918
#include <memory>
#include <savage/fundamental.hpp>
#include <savage/window.hpp>
namespace savage {
	namespace app_managers {
		class app_holder_base {
		public:
			virtual void key_callback(int key, int scancode, int action, int mods) = 0;
			virtual void cursor_position_callback(double x, double y) = 0;
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
			virtual void key_callback(int key, int scancode, int action, int mods) {
				app_.key_callback(key, scancode, action, mods);
			}
			virtual void cursor_position_callback(double x, double y) {
				app_.cursor_position_callback(x, y);
			}
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
			void key_callback(int key, int scancode, int action, int mods) {
				app_holder_->key_callback(key, scancode, action, mods);
			}
			void cursor_position_callback(double x, double y) {
				app_holder_->cursor_position_callback(x, y);
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
	void key_callback(GLFWwindow*, int key, int scancode, int action, int mods) {
		savage::app_manager::instance().key_callback(key, scancode, action, mods);
	}
	void cursor_position_callback(GLFWwindow*, double x, double y) {
		savage::app_manager::instance().cursor_position_callback(x, y);
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

	void init_key_callback() {
		glfwSetKeyCallback(
			savage::window_manager::instance().window().handle(), 
			savage::key_callback
		);
	}
	void init_cursor_position_callback() {
		glfwSetCursorPosCallback(
			savage::window_manager::instance().window().handle(),
			savage::cursor_position_callback
		);
	}
}// namespace savage

#endif //SAVAGE_APP_MANAGER_APP_MANAGER_HPP
