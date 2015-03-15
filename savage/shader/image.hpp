#ifndef SAVAGE_SHADER_IMAGE_HPP
#define SAVAGE_SHADER_IMAGE_HPP
//20140917
#include <vector>
#include <savage/opengl.hpp>
#include <savage/opengl.hpp>
#include <png++/png.hpp>
#include <savage/3rdparty/soil.hpp>

namespace savage {
	namespace shader {
		namespace images {
			class image {
			public:
				image() : image(0,0,std::vector<unsigned char>()){}
				image(
					std::size_t width, std::size_t height, 
					std::vector<unsigned char> const& image_vect
				) :
					width_(width), height_(height), image_vect_(image_vect) {}
				image(
					std::size_t width, std::size_t height, 
					std::vector<unsigned char>&& image_vect
				) :
					width_(width), height_(height), image_vect_(std::move(image_vect)) {}
				std::size_t width() const { return width_; }
				std::size_t height() const { return height_; }
				void const* const address() const {
					return static_cast<void const* const>(image_vect_.data());
				}
			private:
				std::size_t width_, height_;
				std::vector<unsigned char> image_vect_;
			};
		}// namespace images
		using savage::shader::images::image;

		savage::shader::image load_png(boost::filesystem::path const& filename) {
			boost::filesystem::ifstream ifs(filename);
			png::image<png::rgb_pixel> image(ifs);
			std::vector<unsigned char> data;
			/*
			//for(std::size_t y=image.get_height()-1; y > 0; --y) { // bad code
			for(int y=image.get_height()-1; y >= 0; --y) {
				for(std::size_t x=0; x < image.get_width(); ++x) {
					data.push_back(image[y][x].red);
					data.push_back(image[y][x].green);
					data.push_back(image[y][x].blue);
				}
			}
			*/
			std::size_t height = image.get_height();
			for(std::size_t y=0; y < height; ++y) {
				for(std::size_t x=0; x < image.get_width(); ++x) {
					std::size_t back_y = height-y-1;
					data.push_back(image[back_y][x].red);
					data.push_back(image[back_y][x].green);
					data.push_back(image[back_y][x].blue);
				}
			}
			return savage::shader::image(
				image.get_width(), image.get_height(), std::move(data));
		}

		/*
		savage::shader::image load_by_soil(boost::filesystem::path const& filename) {
			int width, height;
			unsigned char* image_ptr =
				SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
			if(!image_ptr) {
				throw "soil image load error";
			}
			auto image = savage::shader::image(width, height, 
				std::vector<unsigned char>(image_ptr, image_ptr+width*height));
			SOIL_free_image_data(image_ptr);
			return image;
		}
		*/

		namespace image_traits {
			template<typename Image>
			struct width {
				static std::size_t call(Image const& image) {
					return image.width();
				}
			};
			template<typename Image>
			struct height {
				static std::size_t call(Image const& image) {
					return image.height();
				}
			};
			template<typename Image>
			struct address {
				static void const* const call(Image const& image) {
					return image.address();
				}
			};
		}// namespace image_traits

		template<typename Image>
		std::size_t image_width(Image const& image) {
			return savage::shader::image_traits::width<Image>::call(image);
		}
		template<typename Image>
		std::size_t image_height(Image const& image) {
			return savage::shader::image_traits::height<Image>::call(image);
		}
		template<typename Image>
		void const* const image_address(Image const& image) {
			return savage::shader::image_traits::address<Image>::call(image);
		}
	}// namespace shader
}// namespace savage

#endif //SAVAGE_SHADER_IMAGE_HPP
