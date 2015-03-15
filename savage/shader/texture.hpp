#ifndef SAVAGE_SHADER_TEXTURE_HPP
#define SAVAGE_SHADER_TEXTURE_HPP
//20140916
#include <savage/opengl.hpp>
#include <savage/shader/image.hpp>
namespace savage {
	namespace shader {
		namespace textures {
			class texture {
				texture(texture const&) = delete;
				texture& operator=(texture const&) = delete;
			public:
				explicit texture(GLint id) : id_(id), handle_() {
					glGenTextures(1, &handle_);
				}
				~texture() {
					glDeleteTextures(1, &handle_);
				}
				GLuint handle() const { return handle_; }
				GLint id() const { return id_; }

			private:
				GLint id_;
				GLuint handle_;
			};
		}// namespace textures
		using savage::shader::textures::texture;

		template<GLint format, typename Image>
		void set_image(savage::shader::texture const& texture, Image const& image) {
			glActiveTexture(GL_TEXTURE0+texture.id());
			glBindTexture(GL_TEXTURE_2D, texture.handle());
			//glPixelStorei(GL_PACK_ALIGNMENT, 1);
			//TODO
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(
				GL_TEXTURE_2D, 
				0, 
				format,
				savage::shader::image_width(image),
				savage::shader::image_height(image), 
				/*must be*/0,
				/*must be*/format,
				GL_UNSIGNED_BYTE,
				savage::shader::image_address(image)
			);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		template<GLint format, typename Image>
		void set_skybox_image(savage::shader::texture const& texture, Image const& image) {
			glActiveTexture(GL_TEXTURE0+texture.id());
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture.handle());
			GLuint targets[] = {
				GL_TEXTURE_CUBE_MAP_POSITIVE_X,
				GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
				GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
				GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
				GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
				GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
			};
			for(std::size_t i = 0; i < 6; ++i) {
				glTexImage2D(
					targets[i], 
					0, 
					format, 
					savage::shader::image_width(image),
					savage::shader::image_height(image), 
					/*must be*/0,
					/*must be*/format,
					GL_UNSIGNED_BYTE,
					savage::shader::image_address(image)
				);
			}
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}

	}// namespace shader
}// namespace savage

#endif //SAVAGE_SHADER_TEXTURE_HPP
