#ifndef SAVAGE_SHADER_PROGRAM_HPP
#define SAVAGE_SHADER_PROGRAM_HPP
//20140916
#include<string>
#include<vector>
#include<savage/opengl.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<savage/shader/object.hpp>
#include<savage/shader/attribute.hpp>
#include<savage/shader/uniform.hpp>
#include<savage/shader/texture.hpp>
namespace savage { 
	namespace shader {
		namespace programs {
			struct create_error : public std::exception {
				create_error() {}
				virtual char const* what() const throw() {
					return "shader program creation failed.";
				}
			};
			class program {
			public:
				program() : handle_(glCreateProgram()) {
					if(!handle_) {
						throw savage::shader::programs::create_error();
					}
				}
				~program() {
					glDeleteProgram(handle_);
				}
				GLuint handle() const { return handle_; }

			private:
				program(program const& program);
				program& operator=(program const& program);
				GLuint handle_;
			};

		}// namespace programs
		using savage::shader::programs::program;

		void attach(
			savage::shader::programs::program const& program, 
			savage::shader::objects::object const& object
		) {
			glAttachShader(program.handle(), object.handle());
		}

		void bind_attribute(
			savage::shader::programs::program const& program,
			savage::shader::attributes::attribute const& attribute
		) {
			glBindAttribLocation(program.handle(), 
				attribute.location(), attribute.name().c_str());
		}

		struct link_error : public std::exception {
			link_error(std::string const& message) : message_(message) {}
			virtual char const* what() const throw() {
				return ("shader program linking failed. \""+message_+"\"").c_str();
			}
		private:
			std::string message_;
		};
		void link(savage::shader::programs::program const& program) {
			glLinkProgram(program.handle());
			GLint status;
			glGetProgramiv(program.handle(), GL_LINK_STATUS, &status);
			if(status == GL_FALSE) {
				GLint loglen;
				glGetProgramiv(program.handle(), GL_INFO_LOG_LENGTH, &loglen);
				std::vector<char> log(loglen, '\0');
				GLsizei written;
				glGetProgramInfoLog(program.handle(), loglen, &written, log.data());
				throw savage::shader::link_error(
					std::string(log.data(), log.data()+written));
			}

		}
		void use(savage::shader::programs::program const& program) {
			glUseProgram(program.handle());
		}

		void set_uniform(
			savage::shader::programs::program const& program, 
			savage::shader::uniform const& uniform,
			glm::mat4 const& data
		) {
			GLuint location = 
				glGetUniformLocation(program.handle(), uniform.name().c_str());
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
		}
		void set_uniform(
			savage::shader::programs::program const& program, 
			savage::shader::uniform const& uniform,
			glm::vec4 const& data
		) {
			GLuint location = 
				glGetUniformLocation(program.handle(), uniform.name().c_str());
			glUniform4fv(location, 1, glm::value_ptr(data));
		}
		void set_uniform(
			savage::shader::programs::program const& program, 
			savage::shader::uniform const& uniform,
			glm::vec3 const& data
		) {
			GLuint location = 
				glGetUniformLocation(program.handle(), uniform.name().c_str());
			glUniform3fv(location, 1, glm::value_ptr(data));
		}
		void set_uniform(
			savage::shader::programs::program const& program, 
			savage::shader::uniform const& uniform,
			int data
		) {
			GLuint location = 
				glGetUniformLocation(program.handle(), uniform.name().c_str());
			glUniform1i(location, data);
		}
		void set_texture_uniform(
			savage::shader::programs::program const& program, 
			savage::shader::uniform const& uniform,
			savage::shader::texture const& texture
		) {
			savage::shader::set_uniform(program, uniform, texture.id());
		}
	}// namespace shader
}// namespace savage

#endif //SAVAGE_SHADER_PROGRAM_HPP
