#ifndef SAVAGE_SHADER_UNIFORM_HPP
#define SAVAGE_SHADER_UNIFORM_HPP
//20140930
#include <string>
namespace savage {
	namespace shader {
		class uniform {
		public:
			uniform(std::string const& type, std::string const& name) : 
				type_(type), name_(name) {}
			std::string const& type() const { return type_; }
			std::string const& name() const { return name_; }
		private:
			std::string type_, name_;
		};
	}// namespace shader
}// namespace savage

#endif //SAVAGE_SHADER_UNIFORM_HPP
