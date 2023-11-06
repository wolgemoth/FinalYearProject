//
// Created by loui on 23/10/23.
//

#include "Texture.h"

namespace LouiEriksson {
	
	Texture::Texture() {
		
		m_TextureID = -1;
		
		m_Width  = -1;
		m_Height = -1;
	}
	
	Texture::Texture(const int& _width, const int& _height, const GLuint& _textureID) {
		
		m_TextureID = _textureID;
		
		m_Width  = _width;
		m_Height = _height;
	}
	
	Texture::Texture(const GLuint& _textureID) {
		
		m_TextureID = _textureID;
		
		m_Width  = -1;
		m_Height = -1;
		
		Texture::Bind(*this);
		
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,  &m_Width );
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &m_Height);
		
		Texture::Unbind();
	}
	
	Texture::Texture(Texture&& _other) noexcept {
	
		m_Width  = _other.m_Width;
		m_Height = _other.m_Height;
		
		m_TextureID = _other.m_TextureID;
		
		_other.m_Width  = -1;
		_other.m_Height = -1;
		
		_other.m_TextureID = -1;
	}
	
	Texture::~Texture() {
		Discard();
	}
	
	const int& Texture::Width() const { return m_Width; }
	
	const int& Texture::Height() const { return m_Height; }
	
	void Texture::Bind(const Texture& _texture) {
		glBindTexture(GL_TEXTURE_2D, _texture.m_TextureID);
	}
	
	void Texture::Unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	void Texture::Discard() const {
		if (m_TextureID > 0) { glDeleteTextures(1, &m_TextureID); }
	}
	
	const GLuint& Texture::ID() const {
		return m_TextureID;
	}
	
	Texture& Texture::operator = (Texture&& _other) noexcept {
		
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
	
	void Texture::Reinitialise(int _width, int _height) {
		throw std::runtime_error("Not implemented!");
	}
	
}