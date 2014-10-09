#ifndef SAVAGE_OPENGL_HPP
#define SAVAGE_OPENGL_HPP
//20140918
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
namespace savage {
	void opengl_init() { glewInit(); }
	void print(std::string const& name, glm::vec3 const& v) {
		std::cout << name << " " << v[0] << " " << v[1] << " " << v[2] << std::endl;
	}
}// namespace savage
#endif //SAVAGE_OPENGL_HPP
