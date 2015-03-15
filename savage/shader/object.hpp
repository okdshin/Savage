#ifndef SAVAGE_SHADER_OBJECT_HPP
#define SAVAGE_SHADER_OBJECT_HPP
//20140916
#include <GL/glew.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
namespace savage {
	namespace shader {
		namespace objects {
			class object {
				object(object const&) = delete;
				object& operator=(object const&) = delete;
			public:
				object(GLuint type) : handle_(glCreateShader(type)) {}
				~object() { glDeleteShader(handle_); }
				GLuint handle() const { return handle_; }
			private:
				GLuint handle_;
			};
		}// namespace objects
		using savage::shader::objects::object;
	
		void load_source(
			savage::shader::objects::object const& object, 
			boost::filesystem::path const& filepath
		) {
			boost::filesystem::ifstream source_file(filepath);
			std::vector<char> source;
			std::copy(
				std::istreambuf_iterator<char>(source_file),
				std::istreambuf_iterator<char>(),
				std::back_inserter(source)
			);
			source.push_back('\0');
			char const* source_array[] = {source.data()};
			glShaderSource(object.handle(), 1, source_array, 0);
		}

		struct compile_error : public std::exception {
			compile_error(std::string const& message) : message_(message) {}
			virtual char const* what() const throw() {
				return ("shader object compling failed. \"" + message_ + "\"").c_str();
			}
		private:
			std::string message_;
		};
		void compile(savage::shader::objects::object const& object) {
			glCompileShader(object.handle());
			GLint result;
			glGetShaderiv(object.handle(), GL_COMPILE_STATUS, &result);
			if(result == GL_FALSE) {
				GLint loglen;
				glGetShaderiv(object.handle(), GL_INFO_LOG_LENGTH, &loglen);
				std::vector<char> log(loglen, '\0');
				GLsizei written;
				glGetShaderInfoLog(object.handle(), loglen, &written, log.data());
				throw savage::shader::compile_error(
					std::string(log.data(), log.data()+written)
				);
			}
		}
	}// namespace shader
}// namespace savage

#endif //SAVAGE_SHADER_OBJECT_HPP
