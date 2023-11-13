//
// Created by loui on 23/10/23.
//

#include "Cubemap.h"

namespace LouiEriksson {
	
	Cubemap::Cubemap() : Texture(-1, -1) {}
	
	Cubemap::Cubemap(const int& _width, const int& _height, const GLuint& _textureID) : Texture(_width, _height, _textureID) {}
	
	Cubemap::Cubemap(Cubemap&& _other) noexcept : Texture(std::move(_other)) {
		
		m_Width  = _other.m_Width;
		m_Height = _other.m_Height;
		
		m_TextureID = _other.m_TextureID;
		
		_other.m_Width  = -1;
		_other.m_Height = -1;
		
		_other.m_TextureID = 0;
	}
	
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
	
	Cubemap& Cubemap::operator = (Cubemap&& _other) noexcept {
		
		if (this != &_other) {
			
			this->m_Width  = _other.m_Width;
			this->m_Height = _other.m_Height;
			
			this->m_TextureID = _other.m_TextureID;
			
			_other.m_Width  = -1;
			_other.m_Height = -1;
			
			_other.m_TextureID = -1;
		}
		
		return *this;
	}
}