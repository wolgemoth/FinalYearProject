//
// Created by loui on 23/10/23.
//

#include "Texture.h"

namespace LouiEriksson {
	
	Texture::Texture() {
		
		m_TextureID = 0;
		
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
	
	void Texture::GetFormatData(const GLenum& _internalFormat, GLenum& _textureFormat, int& _channels) {
		
		if (_internalFormat == GL_R8   || _internalFormat == GL_R16 || _internalFormat == GL_R16F ||
			_internalFormat == GL_R32F || _internalFormat == GL_R32F_EXT
		) {
		    _textureFormat = GL_R;
			_channels      = 1;
		}
		else if (_internalFormat == GL_RG    || _internalFormat == GL_RG8   || _internalFormat == GL_RG16 ||
		         _internalFormat == GL_RG16F || _internalFormat == GL_RG32F || _internalFormat == GL_RG32F_EXT
		) {
		    _textureFormat = GL_RG;
			_channels      = 2;
		}
		else if (_internalFormat == GL_RGB    || _internalFormat == GL_RGB8   || _internalFormat == GL_RGB16      ||
		         _internalFormat == GL_RGB16F || _internalFormat == GL_RGB32F || _internalFormat == GL_RGB32F_ARB ||
				 _internalFormat == GL_RGB32F_EXT
		) {
		    _textureFormat = GL_RGB;
			_channels      = 3;
		}
		else if (_internalFormat == GL_RGBA    || _internalFormat == GL_RGBA8   || _internalFormat == GL_RGBA16      ||
		         _internalFormat == GL_RGBA16F || _internalFormat == GL_RGBA32F || _internalFormat == GL_RGBA32F_ARB ||
				 _internalFormat == GL_RGBA32F_EXT
		) {
		    _textureFormat = GL_RGBA;
			_channels      = 4;
		}
		else {
			
			_textureFormat = GL_NONE;
			_channels      = 0;
			
		    std::cout << "Unknown format \"" << _internalFormat << "\"\n";
		}
	}
}