#ifndef SAVAGE_OPENGL_HPP
#define SAVAGE_OPENGL_HPP
//20140918
#include <GL/glew.h>
namespace savage {
	void opengl_init() { glewInit(); }
}// namespace savage
#endif //SAVAGE_OPENGL_HPP
