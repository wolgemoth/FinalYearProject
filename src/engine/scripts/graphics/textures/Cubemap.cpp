#include "Cubemap.h"

#include "../Texture.h"

#include <GL/glew.h>

namespace LouiEriksson::Engine::Graphics {
	
	Cubemap::Cubemap(const int& _width, const int& _height, const GLuint& _textureID, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode) : Texture(_width, _height, _textureID, _format, _filterMode, _wrapMode) {}
	
	Cubemap::~Cubemap() {
		Discard();
	}
	
} // LouiEriksson::Engine::Graphics