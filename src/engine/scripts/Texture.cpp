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
	
	int Texture::GetChannels(GLenum _format) {
		
			int result = 0;
			
			if (_format == GL_RED  || _format == GL_BLUE || _format == GL_GREEN ||
			    _format == GL_R8   || _format == GL_R16  || _format == GL_R16F  ||
				_format == GL_R32F || _format == GL_R32F_EXT
			) {
			    result = 1;
			}
			else if (_format == GL_RG    || _format == GL_RG8   || _format == GL_RG16 ||
			         _format == GL_RG16F || _format == GL_RG32F || _format == GL_RG32F_EXT
			) {
			    result = 2;
			}
			else if (_format == GL_RGB    || _format == GL_RGB8   || _format == GL_RGB16      ||
			         _format == GL_RGB16F || _format == GL_RGB32F || _format == GL_RGB32F_ARB ||
					 _format == GL_RGB32F_EXT
			) {
			    result = 3;
			}
			else if (_format == GL_RGBA    || _format == GL_RGBA8   || _format == GL_RGBA16      ||
			         _format == GL_RGBA16F || _format == GL_RGBA32F || _format == GL_RGBA32F_ARB ||
					 _format == GL_RGBA32F_EXT
			) {
			    result = 4;
			}
			else {
			    std::cout << "Unknown texture format \"" << _format << "\"\n";
			}
			
			return result;
	}
	
}