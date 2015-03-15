#ifndef SAVAGE_SHADER_DATA_DATA_HPP
#define SAVAGE_SHADER_DATA_DATA_HPP
//20140917
#include <vector>
#include <savage/opengl.hpp>
namespace savage {
	namespace shader {
		namespace datas {
			class data {
			public:
				data(std::size_t dimension, std::vector<float> const& data_vect) :
					dimension_(dimension), data_vect_(data_vect) {}
				data(std::size_t dimension, std::vector<float>&& data_vect) :
					dimension_(dimension), data_vect_(std::move(data_vect)) {}
				std::size_t element_num() const { return data_vect_.size()/dimension_; }
				std::size_t dimension() const { return dimension_; }
				std::size_t size() const { return data_vect_.size()*sizeof(float); }
				void const* const address() const {
					return static_cast<void const* const>(data_vect_.data());
				}
			private:
				std::size_t dimension_;
				std::vector<float> data_vect_;
			};
		}// namespace datas
		using savage::shader::datas::data;

		namespace data_traits {
			template<typename Data>
			struct element_num {
				static std::size_t call(Data const& data) {
					return data.element_num();
				}
			};
			template<typename Data>
			struct dimension {
				static std::size_t call(Data const& data) {
					return data.dimension();
				}
			};
			template<typename Data>
			struct size {
				static GLuint call(Data const& data) {
					return data.size();
				}
			};
			template<typename Data>
			struct address {
				static void const* const call(Data const& data) {
					return data.address();
				}
			};
		}// namespace data_traits

		template<typename Data>
		std::size_t data_element_num(Data const& data) {
			return savage::shader::data_traits::element_num<Data>::call(data);
		}
		template<typename Data>
		std::size_t data_dimension(Data const& data) {
			return savage::shader::data_traits::dimension<Data>::call(data);
		}
		template<typename Data>
		std::size_t data_size(Data const& data) {
			return savage::shader::data_traits::size<Data>::call(data);
		}
		template<typename Data>
		void const* const data_address(Data const& data) {
			return savage::shader::data_traits::address<Data>::call(data);
		}
	}// namespace shader
}// namespace savage

#endif //SAVAGE_SHADER_DATA_DATA_HPP
