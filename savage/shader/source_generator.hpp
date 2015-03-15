#ifndef SAVAGE_SHADER_SOURCE_GENERATOR_HPP
#define SAVAGE_SHADER_SOURCE_GENERATOR_HPP
//20141003
#include <string>
#include <sstream>
#include <vector>
#include <boost/format.hpp>
namespace savage {
	namespace shader {
		//TODO
		class source_generator {
		public:
			source_generator() {}
			void add_comment(std::string const& comment) {
				lines_.emplace_back(std::string("//")+comment+"\n");
			}
			void add_uniform(
				std::string const& type, 
				std::string const& name
			) {
				add_line("uniform", type, name);
			}
			void add_uniform(savage::shader::uniform const& uniform) {
				add_uniform(uniform.type(), uniform.name());
			}
			void add_in_attribute(
				std::string const& type, 
				std::string const& name
			) {
				add_line("in", type, name);
			}
			void add_in_attribute(savage::shader::attribute const& attribute) {
				add_in_attribute(attribute.type(), attribute.name());
			}
			void add_out_attribute(
				std::string const& type, 
				std::string const& name
			) {
				add_line("out", type, name);
			}
			std::string generate_source() const {
				std::ostringstream oss;
				for(auto const& line : lines_) {
					oss << line;
				}
				return oss.str();
			}
		private:
			void add_line(
				std::string const& kind, 
				std::string const& type, 
				std::string const& name
			) {
				lines_.emplace_back(
					boost::str(boost::format("%1% %2% %3%;\n") % kind % type % name)
				);
			}

			std::vector<std::string> lines_;
		};
	}// namespace shader
}// namespace savage

#endif //SAVAGE_SHADER_SOURCE_GENERATOR_HPP
