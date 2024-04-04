#include "Cubemap.h"

#include "../Texture.h"

#include <GL/glew.h>

namespace LouiEriksson::Engine::Graphics {
	
	Cubemap::Cubemap(const int& _width, const int& _height, const GLuint& _textureID, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode) : Texture(_width, _height, _textureID, _format, _filterMode, _wrapMode) {}
	
	Cubemap::~Cubemap() {
		Discard();
	}
	
	void Cubemap::Bind(const Cubemap& _cubemap, const bool& _force) {
		
		if (_force || Cubemap::s_CurrentCubemap != _cubemap.m_TextureID) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, Cubemap::s_CurrentCubemap = static_cast<GLint>(_cubemap.m_TextureID));
		}
	}
	
	void Cubemap::Bind(const GLuint& _cubemap, const bool& _force) {
		
		if (_force || Cubemap::s_CurrentCubemap != _cubemap) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, Cubemap::s_CurrentCubemap = static_cast<GLint>(_cubemap));
		}
	}
	
	void Cubemap::Unbind(const bool& _force) {
		
		if (_force || Cubemap::s_CurrentCubemap != GL_NONE) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, Cubemap::s_CurrentCubemap = GL_NONE);
		}
	}
	
} // LouiEriksson::Engine::Graphics