#ifndef SAVAGE_SHADER_DATA_STD_VECTOR_GLM_VEC2_HPP
#define SAVAGE_SHADER_DATA_STD_VECTOR_GLM_VEC2_HPP
//20141002
#include <vector>
#include <savage/opengl.hpp>
#include <savage/shader/data/data.hpp>
namespace savage {
	namespace shader {
		namespace data_traits {
			// specialization of std::vector<glm::vec2>
			template<>
			struct element_num<std::vector<glm::vec2>> {
				static std::size_t call(std::vector<glm::vec2> const& data) {
					return data.size();
				}
			};
			template<>
			struct dimension<std::vector<glm::vec2>> {
				static std::size_t call(std::vector<glm::vec2> const& data) {
					return 2;
				}
			};
			template<>
			struct size<std::vector<glm::vec2>> {
				static GLuint call(std::vector<glm::vec2> const& data) {
					return data.size()*sizeof(glm::vec2);
				}
			};
			template<>
			struct address<std::vector<glm::vec2>> {
				static void const* const call(std::vector<glm::vec2> const& data) {
					return data.data();
				}
			};
		}// namespace data_traits
	}// namespace shader
}// namespace savage

#endif //SAVAGE_SHADER_DATA_STD_VECTOR_GLM_VEC2_HPP
