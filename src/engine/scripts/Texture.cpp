#include "Texture.h"

#include <utility>

namespace LouiEriksson {
	
	
	Texture::Texture(
		const int& _width, const int& _height, const GLuint& _textureID, Texture::Parameters::Format  _format, Texture::Parameters::FilterMode  _filterMode, Texture::Parameters::WrapMode  _wrapMode) :
		    m_Format(std::move(_format)),
		m_FilterMode(std::move(_filterMode)),
		  m_WrapMode(std::move(_wrapMode))
	{
		m_TextureID = _textureID;
		
		m_Width  = _width;
		m_Height = _height;
	}
	
	Texture::Texture(Texture&& _other) noexcept :
		    m_Format(std::move(_other.m_Format)),
		m_FilterMode(std::move(_other.m_FilterMode)),
		  m_WrapMode(std::move(_other.m_WrapMode))
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
	
	const int& Texture::Width() const { return m_Width; }
	
	const int& Texture::Height() const { return m_Height; }
	
	void Texture::Bind(const Texture& _texture) {
		
		if (s_CurrentTexture != _texture.m_TextureID) {
			s_CurrentTexture  = _texture.m_TextureID;
			
			glBindTexture(GL_TEXTURE_2D, _texture.m_TextureID);
		}
	}
	
	void Texture::Unbind() {
		
		if (s_CurrentTexture != GL_NONE) {
			s_CurrentTexture = GL_NONE;
			
			glBindTexture(GL_TEXTURE_2D, GL_NONE);
		}
	}
	
	void Texture::Discard() const {
		if (m_TextureID > 0) { glDeleteTextures(1, &m_TextureID); }
	}
	
	const GLuint& Texture::ID() const {
		return m_TextureID;
	}
	
	void Texture::GetFormatData(const GLenum& _internalFormat, GLenum& _textureFormat, int& _channels) {
		
		if (_internalFormat == GL_R    || _internalFormat == GL_R8   || _internalFormat == GL_R16 ||
			_internalFormat == GL_R16F || _internalFormat == GL_R32F || _internalFormat == GL_R32F_EXT
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
		else if (_internalFormat == GL_RGB        || _internalFormat == GL_SRGB       || _internalFormat == GL_SRGB8        ||
				 _internalFormat == GL_SRGB_EXT   || _internalFormat == GL_RGB8       || _internalFormat == GL_RGB10        ||
				 _internalFormat == GL_RGB10_A2   || _internalFormat == GL_RGB10_A2UI || _internalFormat == GL_RGB10_A2_EXT ||
				 _internalFormat == GL_RGB10_EXT  || _internalFormat == GL_RGB12      || _internalFormat == GL_RGB12_EXT    ||
				 _internalFormat == GL_RGB16      || _internalFormat == GL_RGB16F     || _internalFormat == GL_RGB32F       ||
				 _internalFormat == GL_RGB32F_ARB || _internalFormat == GL_RGB32F_EXT
		) {
		    _textureFormat = GL_RGB;
			_channels      = 3;
		}
		else if (_internalFormat == GL_SRGB_ALPHA || _internalFormat == GL_SRGB8_ALPHA8 || _internalFormat == GL_SRGB8_ALPHA8_EXT ||
				 _internalFormat == GL_RGBA       || _internalFormat == GL_RGBA8        || _internalFormat == GL_RGBA12           ||
				 _internalFormat == GL_RGBA12_EXT || _internalFormat == GL_RGBA16       || _internalFormat == GL_RGBA16F          ||
				 _internalFormat == GL_RGBA32F    || _internalFormat == GL_RGBA32F_ARB  || _internalFormat == GL_RGBA32F_EXT
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
	
	const Texture::Parameters::Format&     Texture::Format()     const { return m_Format; }
	const Texture::Parameters::FilterMode& Texture::FilterMode() const { return m_FilterMode; }
	const Texture::Parameters::WrapMode&   Texture::WrapMode()   const { return m_WrapMode; }
	
	Texture::Parameters::Format::Format(const GLenum& _pixelFormat, bool _mips) {
		
		m_Mips = _mips;
		
		Texture::GetFormatData(_pixelFormat, m_TextureFormat, m_Channels);
		
		m_PixelFormat = m_TextureFormat == GL_NONE ? GL_NONE : _pixelFormat;
	}
	
	Texture::Parameters::Format::Format(const Texture::Parameters::Format& _other) {
		m_PixelFormat   = _other.m_PixelFormat;
		m_TextureFormat = _other.m_TextureFormat;
		m_Channels      = _other.m_Channels;
		m_Mips          = _other.m_Mips;
	}
	
	Texture::Parameters::Format& Texture::Parameters::Format::operator = (const Texture::Parameters::Format& _other) {
		
		m_PixelFormat   = _other.m_PixelFormat;
		m_TextureFormat = _other.m_TextureFormat;
		m_Channels      = _other.m_Channels;
		m_Mips          = _other.m_Mips;
		
		return *this;
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
	
	Texture::Parameters::Format& Texture::Parameters::Format::operator = (Texture::Parameters::Format&& _other) noexcept {
		
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
		
		return *this;
	}
	
	const GLenum& Texture::Parameters::Format::PixelFormat()   const { return   m_PixelFormat; }
	const GLenum& Texture::Parameters::Format::TextureFormat() const { return m_TextureFormat; }
	const    int& Texture::Parameters::Format::Channels()      const { return      m_Channels; }
	const   bool& Texture::Parameters::Format::Mips()          const { return          m_Mips; }
	
	Texture::Parameters::FilterMode::FilterMode(const GLenum& _min, const GLenum& _mag) {
		m_Min = _min;
		m_Mag = _mag;
	}
	
	Texture::Parameters::FilterMode::FilterMode(const Texture::Parameters::FilterMode& _other) {
		m_Min = _other.m_Min;
		m_Mag = _other.m_Mag;
	}
	
	Texture::Parameters::FilterMode& Texture::Parameters::FilterMode::operator = (const Texture::Parameters::FilterMode& _other) {
		
		if (&_other != this) {
			m_Min = _other.m_Min;
			m_Mag = _other.m_Mag;
		}
		
		return *this;
	}
	
	Texture::Parameters::FilterMode::FilterMode(Texture::Parameters::FilterMode&& _other) noexcept {
	
		if (&_other != this) {
			m_Min = _other.m_Min;
			m_Mag = _other.m_Mag;
			
			_other.m_Min = GL_NONE;
			_other.m_Mag = GL_NONE;
		}
	}
	
	Texture::Parameters::FilterMode& Texture::Parameters::FilterMode::operator = (Texture::Parameters::FilterMode&& _other) noexcept {
		
		if (&_other != this) {
			m_Min = _other.m_Min;
			m_Mag = _other.m_Mag;
			
			_other.m_Min = GL_NONE;
			_other.m_Mag = GL_NONE;
		}
		return *this;
	}
	
	const GLenum& Texture::Parameters::FilterMode::Min() const { return m_Min; }
	const GLenum& Texture::Parameters::FilterMode::Mag() const { return m_Mag; }
	
	Texture::Parameters::WrapMode::WrapMode(const GLenum& _s, const GLenum& _t, const GLenum& _r) {
		m_WrapS = _s;
		m_WrapT = _t;
		m_WrapR = _r;
	}
	
	Texture::Parameters::WrapMode::WrapMode(const Texture::Parameters::WrapMode& _other) {
		
		m_WrapS = _other.m_WrapS;
		m_WrapT = _other.m_WrapT;
		m_WrapR = _other.m_WrapR;
	}
	
	Texture::Parameters::WrapMode& Texture::Parameters::WrapMode::operator = (const Texture::Parameters::WrapMode& _other) {
		
		m_WrapS = _other.m_WrapS;
		m_WrapT = _other.m_WrapT;
		m_WrapR = _other.m_WrapR;
		
		return *this;
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
	
	Texture::Parameters::WrapMode& Texture::Parameters::WrapMode::operator = (Texture::Parameters::WrapMode&& _other) noexcept {
		
		if (&_other != this) {
			m_WrapS = _other.m_WrapS;
			m_WrapT = _other.m_WrapT;
			m_WrapR = _other.m_WrapR;
			
			_other.m_WrapS = GL_NONE;
			_other.m_WrapT = GL_NONE;
			_other.m_WrapR = GL_NONE;
		}
		
		return *this;
	}
	
	const GLenum& Texture::Parameters::WrapMode::WrapS() const { return m_WrapS; }
	const GLenum& Texture::Parameters::WrapMode::WrapT() const { return m_WrapT; }
	const GLenum& Texture::Parameters::WrapMode::WrapR() const { return m_WrapR; }
	
	
}