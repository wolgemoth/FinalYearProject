//
// Created by loui on 23/10/23.
//

#include "Cubemap.h"

namespace LouiEriksson {
	
	Cubemap::Cubemap(const int& _width, const int& _height, const GLuint& _textureID, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode) : Texture(_width, _height, _textureID, _format, _filterMode, _wrapMode) {}
	
	Cubemap::~Cubemap() {
		Discard();
	}
	
	void Cubemap::Bind(const Cubemap& _cubemap) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemap.m_TextureID);
	}
	
	void Cubemap::Unbind() {
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	
	void Cubemap::Discard() const {
		Texture::Discard();
	}
}