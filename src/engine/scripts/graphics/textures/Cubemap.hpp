#ifndef FINALYEARPROJECT_CUBEMAP_HPP
#define FINALYEARPROJECT_CUBEMAP_HPP

#include "../Texture.hpp"

#include <GL/glew.h>

namespace LouiEriksson::Engine {
	
	class File;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Graphics {
	
	class Cubemap final : public Texture {
	
		friend LouiEriksson::Engine::Resources;
		
		friend class Camera;
		
	private:
		
		inline static GLuint s_CurrentCubemap { GL_NONE };
		
		explicit Cubemap(const int& _width, const int& _height, const GLuint& _textureID, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode) noexcept
			: Texture(_width, _height, _textureID, _format, _filterMode, _wrapMode) {}
		
	public:
		
		~Cubemap() {
			Discard();
		}
		
		static void Bind(const Cubemap& _cubemap, const bool& _force = false) {
			
			if (_force || Cubemap::s_CurrentCubemap != _cubemap.m_TextureID) {
				glBindTexture(GL_TEXTURE_CUBE_MAP, Cubemap::s_CurrentCubemap = static_cast<const GLint>(_cubemap.m_TextureID));
			}
		}
		
		static void Bind(const GLuint& _cubemap, const bool& _force = false) {
			
			if (_force || Cubemap::s_CurrentCubemap != _cubemap) {
				glBindTexture(GL_TEXTURE_CUBE_MAP, Cubemap::s_CurrentCubemap = static_cast<const GLint>(_cubemap));
			}
		}
		
		static void Unbind(const bool& _force = false) {
			
			if (_force || Cubemap::s_CurrentCubemap != GL_NONE) {
				glBindTexture(GL_TEXTURE_CUBE_MAP, Cubemap::s_CurrentCubemap = GL_NONE);
			}
		}
		
		Cubemap(             const Cubemap& _other) = delete;
		Cubemap& operator = (const Cubemap& _other) = delete;
		
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_CUBEMAP_HPP