#ifndef SAVAGE_FUNDAMENTAL_HPP
#define SAVAGE_FUNDAMENTAL_HPP
//20140918
#include <savage/opengl.hpp>
#include <GLFW/glfw3.h>
namespace savage {
	struct opengl_utility_init_error : public std::exception {
		virtual char const* what() const throw() {
			return "opengl utility initializing failed.";
		}
	};
	void opengl_utility_init() {
		if(!glfwInit()) { throw opengl_utility_init_error(); }
	}
	void poll_events() { glfwPollEvents(); }
	void terminate() { glfwTerminate(); }
}// namespace savage

#endif //SAVAGE_FUNDAMENTAL_HPP
