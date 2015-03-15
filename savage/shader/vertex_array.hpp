#ifndef SAVAGE_SHADER_VERTEX_ARRAY_HPP
#define SAVAGE_SHADER_VERTEX_ARRAY_HPP
//20140916
#include <savage/opengl.hpp>
namespace savage {
	namespace shader {
		namespace vertex_arrays {
			class vertex_array {
				vertex_array(vertex_array const&) = delete;
				vertex_array& operator=(vertex_array const&) = delete;
			public:
				vertex_array() : handle_() {
					glGenVertexArrays(1, &handle_);
				}
				~vertex_array() {
					glDeleteVertexArrays(1, &handle_);
				}
				GLuint handle() const { return handle_; }

			private:
				GLuint handle_;
			};
		}// namespace vertex_arrays
		using savage::shader::vertex_arrays::vertex_array;
		void bind(savage::shader::vertex_array const& vertex_array) {
			glBindVertexArray(vertex_array.handle());
		}
	}// namespace shader
}// namespace savage

#endif //SAVAGE_SHADER_VERTEX_ARRAY_HPP
