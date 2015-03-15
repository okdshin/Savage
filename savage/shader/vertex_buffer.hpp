#ifndef SAVAGE_SHADER_VERTEX_BUFFER_HPP
#define SAVAGE_SHADER_VERTEX_BUFFER_HPP
//20140916
#include <savage/opengl.hpp>
#include <savage/shader/data.hpp>
#include <savage/shader/attribute.hpp>
namespace savage {
	namespace shader {
		namespace vertex_buffers {
			class vertex_buffer {
				vertex_buffer(vertex_buffer const&) = delete;
				vertex_buffer& operator=(vertex_buffer const&) = delete;
			public:
				vertex_buffer() : handle_() {
					glGenBuffers(1, &handle_);
				}
				~vertex_buffer() {
					glDeleteBuffers(1, &handle_);
				}
				GLuint handle() const { return handle_; }

			private:
				GLuint handle_;
			};
		}// namespace vertex_buffers
		using savage::shader::vertex_buffers::vertex_buffer;

		template<GLuint target, GLuint usage>
		void set_data(
			savage::shader::vertex_buffer const& vertex_buffer,
			GLuint location, 
			std::size_t dimension, 
			std::size_t data_size, 
			void const* const data
		) {
			glBindBuffer(target, vertex_buffer.handle());
			glBufferData(target, data_size, data, usage);
			glVertexAttribPointer(location, dimension, GL_FLOAT, GL_FALSE, 0, 0);
		}
		
		template<GLuint target, GLuint usage, typename Data>
		void set_data(
			savage::shader::vertex_buffer const& vertex_buffer,
			savage::shader::attributes::attribute const& attribute,
			Data const& data
		) {
			savage::shader::set_data<target, usage>(
				vertex_buffer, 
				attribute.location(), 
				savage::shader::data_dimension(data), 
				savage::shader::data_size(data), 
				savage::shader::data_address(data)
			); 
		}
	}// namespace shader
}// namespace savage

#endif //SAVAGE_SHADER_VERTEX_BUFFER_HPP
