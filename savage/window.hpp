#ifndef SAVAGE_WINDOW_HPP
#define SAVAGE_WINDOW_HPP
//20140918
#include <savage/opengl.hpp>
#include <savage/app_manager.hpp>
namespace savage {
	namespace windows {
		struct creation_error : public std::exception {
			creation_error() {}
			virtual char const* what() const throw() {
				return "window creation failed.";
			}
		};
		class window {
			window(window const&) = delete;
			window& operator=(window const&) = delete;
		public:
			window(
				std::string const& title, 
				std::size_t width, std::size_t height, 
				bool is_fullscreen
			) : 
				handle_(glfwCreateWindow(
					width, height, title.c_str(),
					is_fullscreen ? glfwGetPrimaryMonitor() : 0,
					0
				)),
				width_(width), height_(height)
			{
				if(!handle_) {
					throw savage::windows::creation_error(); 
				}
			}
			~window() { glfwDestroyWindow(handle_); }

			GLFWwindow* const handle() const { return handle_; }

			unsigned int width() const { return width_; }
			unsigned int height() const { return height_; }

		private:
			GLFWwindow* const handle_;
			unsigned int width_, height_;
		};
		class window_manager {
			window_manager(window_manager const&) = delete;
			window_manager& operator=(window_manager const&) = delete;
		public:
			static window_manager& instance() {
				static window_manager self_;
				return self_;
			}
			void initialize(
				std::string const& title, 
				std::size_t width, std::size_t height, 
				bool is_fullscreen
			) {
				window_.reset(
					new savage::windows::window(
						title, width, height, is_fullscreen
					)
				);
			}
			savage::windows::window& window() { return *window_; }
		private:
			window_manager() : window_() {}
			std::unique_ptr<savage::windows::window> window_;
		};
	}// namespac windows
	using savage::windows::window;
	using savage::windows::window_manager;
	void make_context_current(savage::windows::window const& window) {
		glfwMakeContextCurrent(window.handle());
	}
	bool is_window_closed(savage::windows::window const& window) {
		return glfwWindowShouldClose(window.handle());
	}
	void swap_buffers(savage::windows::window const& window) {
		glfwSwapBuffers(window.handle());
	}
}// namespace savage

#endif //SAVAGE_WINDOW_HPP
