#ifndef SAVAGE_SHADER_UTILITY_HPP
#define SAVAGE_SHADER_UTILITY_HPP
//20140916
#include <savage/shader/program.hpp>
#include <savage/shader/object.hpp>
#include <savage/shader/attribute.hpp>
#include <savage/shader/vertex_buffer.hpp>
namespace savage {
	namespace shader {
		template<GLuint type>
		void attach(
			savage::shader::programs::program& program, 
			boost::filesystem::path const& object_filepath
		) {
			savage::shader::objects::object object(type);

			savage::shader::load_source(object, object_filepath);
			savage::shader::compile(object);
			savage::shader::attach(program, object);
		}
	}// namespace shader
}// namespace savage

#endif //SAVAGE_SHADER_UTILITY_HPP
