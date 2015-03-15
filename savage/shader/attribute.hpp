#ifndef SAVAGE_SHADER_ATTRIBUTE_HPP
#define SAVAGE_SHADER_ATTRIBUTE_HPP
//20140917
#include <savage/opengl.hpp>
namespace savage {
	namespace shader {
		namespace attributes {
			class attribute {
			public:
				attribute(
					GLuint location, 
					std::string const& type, 
					std::string const& name
				) : location_(location), type_(type), name_(name) {}
				GLuint location() const { return location_; }
				std::string type() const { return type_; }
				std::string name() const { return name_; }
			private:
				GLuint location_;
				std::string type_, name_;
			};
			void enable_vertex_attribute(
					savage::shader::attributes::attribute const& attribute) {
				glEnableVertexAttribArray(attribute.location());
			}
		}// namespace attributes
		using savage::shader::attributes::attribute;
		using savage::shader::attributes::enable_vertex_attribute;
	}// namespace shader
}// namespace savage

#endif //SAVAGE_SHADER_ATTRIBUTE_HPP
