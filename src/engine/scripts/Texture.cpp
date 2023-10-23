//
// Created by loui on 23/10/23.
//

#include "Texture.h"

namespace LouiEriksson {
	
	Texture::Texture(const int& _width, const int& _height) {
		
		m_TextureID = -1;
		
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
	
	Texture::~Texture() {
		Discard();
	}
	
	Texture Texture::Create(std::string& _path) {
		
		
		return Texture(-1, -1);
	}
	
	const int& Texture::Width() const { return m_Width; }
	
	const int& Texture::Height() const { return m_Height; }
	
	void Texture::Bind(const Texture& _texture) {
		glBindTexture(GL_FRAMEBUFFER, _texture.m_TextureID);
	}
	
	void Texture::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	void Texture::Discard() const {
		if (m_TextureID > 0) { glDeleteTextures(1, &m_TextureID); }
	}
	
	const GLuint& Texture::ID() const {
		return m_TextureID;
	}
	
}