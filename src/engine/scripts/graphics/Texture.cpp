#include "Texture.h"

#include "../core/Debug.h"

#include <GL/glew.h>

#include <string>
#include <utility>

namespace LouiEriksson::Engine::Graphics {
	
	Texture::Texture(const int& _width, const int& _height, const GLuint& _textureID, Texture::Parameters::Format  _format, Texture::Parameters::FilterMode  _filterMode, Texture::Parameters::WrapMode  _wrapMode) noexcept :
		m_Width     (_width    ),
		m_Height    (_height   ),
		m_TextureID (_textureID),
		m_Format    (std::move(_format    )),
		m_FilterMode(std::move(_filterMode)),
		m_WrapMode  (std::move(_wrapMode  )) {}
	
	Texture::Texture(Texture&& _other) noexcept :
		    m_Format(std::move(_other.m_Format    )),
		m_FilterMode(std::move(_other.m_FilterMode)),
		  m_WrapMode(std::move(_other.m_WrapMode  ))
	{
		if (&_other != this) {
			
			m_Width  = _other.m_Width;
			m_Height = _other.m_Height;
			
			m_TextureID = _other.m_TextureID;
			
			_other.m_Width  = GL_NONE;
			_other.m_Height = GL_NONE;
			
			_other.m_TextureID = GL_NONE;
		}
	}
	
	Texture& Texture::operator = (Texture&& _other) noexcept {
		
		if (this != &_other) {
			
			this->m_Width  = _other.m_Width;
			this->m_Height = _other.m_Height;
			
			this->m_TextureID = _other.m_TextureID;
			
			this->m_Format     = std::move(_other.m_Format);
			this->m_FilterMode = std::move(_other.m_FilterMode);
			this->m_WrapMode   = std::move(_other.m_WrapMode);
			
			_other.m_Width  = GL_NONE;
			_other.m_Height = GL_NONE;
			
			_other.m_TextureID = GL_NONE;
		}
		
		return *this;
	}
	
	Texture::~Texture() {
		Discard();
	}
	
	void Texture::Bind(const Texture& _texture, const bool& _force) {
		
		if (_force || Texture::s_CurrentTexture != _texture.m_TextureID) {
			glBindTexture(GL_TEXTURE_2D, s_CurrentTexture = static_cast<GLint>(_texture.m_TextureID));
		}
	}
	
	void Texture::Bind(const GLuint& _texture, const bool& _force) {
		
		if (_force || Texture::s_CurrentTexture != _texture) {
			glBindTexture(GL_TEXTURE_2D, s_CurrentTexture = static_cast<GLint>(_texture));
		}
	}
	
	void Texture::Unbind(const bool& _force) {
		
		if (_force || Texture::s_CurrentTexture != GL_NONE) {
			glBindTexture(GL_TEXTURE_2D, s_CurrentTexture = GL_NONE);
		}
	}
	
	void Texture::Discard() const {
		if (m_TextureID != GL_NONE) {
			Texture::Unbind();
			glDeleteTextures(1, &m_TextureID);
		}
	}
	
	void Texture::GetFormatData(const GLenum& _pixelFormat, GLenum& _textureFormat, int& _channels) {
	
		if (_pixelFormat == GL_R    || _pixelFormat == GL_R8   || _pixelFormat == GL_R16 ||
			_pixelFormat == GL_R16F || _pixelFormat == GL_R32F || _pixelFormat == GL_R32F_EXT
		) {
		    _textureFormat = GL_R;
			_channels      = 1;
		}
		else if (_pixelFormat == GL_RG    || _pixelFormat == GL_RG8   || _pixelFormat == GL_RG16 ||
		         _pixelFormat == GL_RG16F || _pixelFormat == GL_RG32F || _pixelFormat == GL_RG32F_EXT
		) {
		    _textureFormat = GL_RG;
			_channels      = 2;
		}
		else if (_pixelFormat == GL_RGB        || _pixelFormat == GL_SRGB       || _pixelFormat == GL_SRGB8        ||
				 _pixelFormat == GL_SRGB_EXT   || _pixelFormat == GL_RGB8       || _pixelFormat == GL_RGB10        ||
				 _pixelFormat == GL_RGB10_A2   || _pixelFormat == GL_RGB10_A2UI || _pixelFormat == GL_RGB10_A2_EXT ||
				 _pixelFormat == GL_RGB10_EXT  || _pixelFormat == GL_RGB12      || _pixelFormat == GL_RGB12_EXT    ||
				 _pixelFormat == GL_RGB16      || _pixelFormat == GL_RGB16F     || _pixelFormat == GL_RGB32F       ||
				 _pixelFormat == GL_RGB32F_ARB || _pixelFormat == GL_RGB32F_EXT
		) {
		    _textureFormat = GL_RGB;
			_channels      = 3;
		}
		else if (_pixelFormat == GL_SRGB_ALPHA || _pixelFormat == GL_SRGB8_ALPHA8 || _pixelFormat == GL_SRGB8_ALPHA8_EXT ||
				 _pixelFormat == GL_RGBA       || _pixelFormat == GL_RGBA8        || _pixelFormat == GL_RGBA12           ||
				 _pixelFormat == GL_RGBA12_EXT || _pixelFormat == GL_RGBA16       || _pixelFormat == GL_RGBA16F          ||
				 _pixelFormat == GL_RGBA32F    || _pixelFormat == GL_RGBA32F_ARB  || _pixelFormat == GL_RGBA32F_EXT
		) {
		    _textureFormat = GL_RGBA;
			_channels      = 4;
		}
		else if (_pixelFormat == GL_DEPTH_COMPONENT) {
			_textureFormat = GL_DEPTH_COMPONENT;
			_channels      = 1;
		}
		else if (_pixelFormat == GL_DEPTH_COMPONENT16) {
			_textureFormat = GL_DEPTH_COMPONENT16;
			_channels      = 1;
		}
		else {
			_textureFormat = GL_NONE;
			_channels      = 0;
			
			Debug::Log("Unknown format \"" + std::to_string(_pixelFormat) + "\"", LogType::Error);
		}
	}
	
	Texture::Parameters::Format::Format(const GLenum& _pixelFormat, const bool& _mips) noexcept :
			m_Mips(_mips)
	{
		Texture::GetFormatData(_pixelFormat, m_TextureFormat, m_Channels);
		
		m_PixelFormat = m_TextureFormat == GL_NONE ? GL_NONE : _pixelFormat;
	}
	
	Texture::Parameters::Format::Format(Texture::Parameters::Format&& _other) noexcept {
	
		if (&_other != this) {
			
			m_PixelFormat   = _other.m_PixelFormat;
			m_TextureFormat = _other.m_TextureFormat;
			m_Channels      = _other.m_Channels;
			m_Mips          = _other.m_Mips;
			
			_other.m_PixelFormat   = GL_NONE;
			_other.m_TextureFormat = GL_NONE;
			_other.m_Channels      = 0;
			_other.m_Mips          = false;
		}
	}
	
	Texture::Parameters::FilterMode::FilterMode(Texture::Parameters::FilterMode&& _other) noexcept {
	
		if (&_other != this) {
			
			m_Min = _other.m_Min;
			m_Mag = _other.m_Mag;
			
			_other.m_Min = GL_NONE;
			_other.m_Mag = GL_NONE;
		}
	}
	
	Texture::Parameters::WrapMode::WrapMode(Texture::Parameters::WrapMode&& _other) noexcept {
		
		if (&_other != this) {
			
			m_WrapS = _other.m_WrapS;
			m_WrapT = _other.m_WrapT;
			m_WrapR = _other.m_WrapR;
			
			_other.m_WrapS = GL_NONE;
			_other.m_WrapT = GL_NONE;
			_other.m_WrapR = GL_NONE;
		}
	}
	
	
} // LouiEriksson::Engine::Graphics