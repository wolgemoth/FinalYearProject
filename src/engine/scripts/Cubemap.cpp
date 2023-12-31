#include "stdafx.h"

#include "Cubemap.h"

// @Assessor: This class was submitted for GACP. Please don't mark it for GEP.

namespace LouiEriksson {
	
	Cubemap::Cubemap(const int& _width, const int& _height, const GLuint& _textureID, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode) : Texture(_width, _height, _textureID, _format, _filterMode, _wrapMode) {}
	
	Cubemap::~Cubemap() {
		Discard();
	}
}