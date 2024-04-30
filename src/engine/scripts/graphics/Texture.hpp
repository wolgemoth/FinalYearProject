#ifndef FINALYEARPROJECT_TEXTURE_HPP
#define FINALYEARPROJECT_TEXTURE_HPP

#include <GL/glew.h>

namespace LouiEriksson::Engine {
	
	class File;
	class Resources;
	class Settings;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Graphics {
	
	/**
	 * @class Texture
	 * @brief Represents a texture.
	 */
	class Texture {
		
		friend LouiEriksson::Engine::File;
		friend LouiEriksson::Engine::Resources;
		friend LouiEriksson::Engine::Settings;
	
		friend class Camera;
		friend class Cubemap;
		friend class Light;
		friend class RenderTexture;
		
	public:
		
		struct Parameters final {
			
			/**
			 * @brief Specifies a Texture's format.
			 */
			struct Format final {
			
			private:
				
				GLenum m_PixelFormat;
				GLenum m_TextureFormat;
				
				int m_Channels;
				
				bool m_Mips;
				
			public:
				
				Format(const GLenum& _pixelFormat, const bool& _mips) noexcept : m_Mips(_mips) {
					
					Texture::GetFormatData(_pixelFormat, m_TextureFormat, m_Channels);
					
					m_PixelFormat = m_TextureFormat == GL_NONE ? GL_NONE : _pixelFormat;
				}
				
				Format             (const Format& _other) = default;
				Format& operator = (const Format& _other) = default;
				
				Format (Format&& _other) noexcept  {
				
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
				
				Format& operator = (Format&& _other) noexcept = default;
				
				[[nodiscard]] constexpr const GLenum&   PixelFormat() const noexcept { return   m_PixelFormat; }
				[[nodiscard]] constexpr const GLenum& TextureFormat() const noexcept { return m_TextureFormat; }
				[[nodiscard]] constexpr const    int&      Channels() const noexcept { return      m_Channels; }
				[[nodiscard]] constexpr const   bool&          Mips() const noexcept { return          m_Mips; }
			};
			
			/**
			 * @brief Specifies a Texture's filter mode.
			 */
			struct FilterMode final {
			
			private:
				GLenum m_Min;
				GLenum m_Mag;
				
			public:
				
				constexpr FilterMode(const GLenum& _min, const GLenum& _mag) noexcept :
					m_Min(_min),
					m_Mag(_mag) {}
				
				FilterMode             (const FilterMode& _other) = default;
				FilterMode& operator = (const FilterMode& _other) = default;
				
				FilterMode (FilterMode&& _other) noexcept {
				
					if (&_other != this) {
						
						m_Min = _other.m_Min;
						m_Mag = _other.m_Mag;
						
						_other.m_Min = GL_NONE;
						_other.m_Mag = GL_NONE;
					}
				}
				
				FilterMode& operator = (FilterMode&& _other) noexcept = default;
				
				[[nodiscard]] constexpr const GLenum& Min() const noexcept { return m_Min; }
				[[nodiscard]] constexpr const GLenum& Mag() const noexcept { return m_Mag; }
			};
			
			/**
			 * @brief Specifies a Texture's wrap mode.
			 */
			struct WrapMode final {
			
			private:
				GLenum m_WrapS;
				GLenum m_WrapT;
				GLenum m_WrapR;
				
			public:
				
				constexpr WrapMode(const GLenum& _s, const GLenum& _t = GL_NONE, const GLenum& _r = GL_NONE) noexcept :
					m_WrapS(_s),
					m_WrapT(_t),
					m_WrapR(_r) {}
				
				WrapMode             (const WrapMode& _other) = default;
				WrapMode& operator = (const WrapMode& _other) = default;
				
				WrapMode (WrapMode&& _other) noexcept {
				
					if (&_other != this) {
						
						m_WrapS = _other.m_WrapS;
						m_WrapT = _other.m_WrapT;
						m_WrapR = _other.m_WrapR;
						
						_other.m_WrapS = GL_NONE;
						_other.m_WrapT = GL_NONE;
						_other.m_WrapR = GL_NONE;
					}
				}
				
				WrapMode& operator = (WrapMode&& _other) noexcept = default;
				
				[[nodiscard]] constexpr const GLenum& WrapS() const noexcept { return m_WrapS; }
				[[nodiscard]] constexpr const GLenum& WrapT() const noexcept { return m_WrapT; }
				[[nodiscard]] constexpr const GLenum& WrapR() const noexcept { return m_WrapR; }
			};
			
		};
		
		~Texture() {
			Discard();
		}

		Texture             (const Texture&  _other) = delete;
		Texture& operator = (const Texture&  _other) = delete;

		Texture (Texture&& _other) noexcept :
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
		
		Texture& operator = (Texture&& _other) noexcept {
			
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
		
		[[nodiscard]] constexpr const int&  Width() const noexcept { return m_Width;  }
		[[nodiscard]] constexpr const int& Height() const noexcept { return m_Height; }
		
		[[nodiscard]] constexpr const Parameters::Format&         Format() const noexcept { return m_Format;     }
		[[nodiscard]] constexpr const Parameters::FilterMode& FilterMode() const noexcept { return m_FilterMode; }
		[[nodiscard]] constexpr const Parameters::WrapMode&     WrapMode() const noexcept { return m_WrapMode;   }
		
		/**
		 * @brief Determine a Texture's format and number of channels using its pixel format
		 *
		 * @param[in] _pixelFormat The pixel format of the texture
		 * @param[out] _textureFormat The texture format (updated by the function)
		 * @param[out] _channels The number of channels (updated by the function)
		 */
		static void GetFormatData(const GLenum& _pixelFormat, GLenum& _textureFormat, int& _channels) {
		
			if (_pixelFormat == static_cast<GLenum>(GL_R   ) || _pixelFormat == static_cast<GLenum>(GL_R8  ) || _pixelFormat == static_cast<GLenum>(GL_R16     ) ||
				_pixelFormat == static_cast<GLenum>(GL_R16F) || _pixelFormat == static_cast<GLenum>(GL_R32F) || _pixelFormat == static_cast<GLenum>(GL_R32F_EXT)
			) {
			    _textureFormat = GL_R;
				_channels      = 1;
			}
			else if (_pixelFormat == static_cast<GLenum>(GL_RG   ) || _pixelFormat == static_cast<GLenum>(GL_RG8  ) || _pixelFormat == static_cast<GLenum>(GL_RG16)      ||
			         _pixelFormat == static_cast<GLenum>(GL_RG16F) || _pixelFormat == static_cast<GLenum>(GL_RG32F) || _pixelFormat == static_cast<GLenum>(GL_RG32F_EXT)
			) {
			    _textureFormat = GL_RG;
				_channels      = 2;
			}
			else if (_pixelFormat == static_cast<GLenum>(GL_RGB       ) || _pixelFormat == static_cast<GLenum>(GL_SRGB      ) || _pixelFormat == static_cast<GLenum>(GL_SRGB8       ) ||
					 _pixelFormat == static_cast<GLenum>(GL_SRGB_EXT  ) || _pixelFormat == static_cast<GLenum>(GL_RGB8      ) || _pixelFormat == static_cast<GLenum>(GL_RGB10       ) ||
					 _pixelFormat == static_cast<GLenum>(GL_RGB10_A2  ) || _pixelFormat == static_cast<GLenum>(GL_RGB10_A2UI) || _pixelFormat == static_cast<GLenum>(GL_RGB10_A2_EXT) ||
					 _pixelFormat == static_cast<GLenum>(GL_RGB10_EXT ) || _pixelFormat == static_cast<GLenum>(GL_RGB12     ) || _pixelFormat == static_cast<GLenum>(GL_RGB12_EXT   ) ||
					 _pixelFormat == static_cast<GLenum>(GL_RGB16     ) || _pixelFormat == static_cast<GLenum>(GL_RGB16F    ) || _pixelFormat == static_cast<GLenum>(GL_RGB32F      ) ||
					 _pixelFormat == static_cast<GLenum>(GL_RGB32F_ARB) || _pixelFormat == static_cast<GLenum>(GL_RGB32F_EXT)
			) {
			    _textureFormat = GL_RGB;
				_channels      = 3;
			}
			else if (_pixelFormat == static_cast<GLenum>(GL_SRGB_ALPHA) || _pixelFormat == static_cast<GLenum>(GL_SRGB8_ALPHA8) || _pixelFormat == static_cast<GLenum>(GL_SRGB8_ALPHA8_EXT) ||
					 _pixelFormat == static_cast<GLenum>(GL_RGBA      ) || _pixelFormat == static_cast<GLenum>(GL_RGBA8       ) || _pixelFormat == static_cast<GLenum>(GL_RGBA12          ) ||
					 _pixelFormat == static_cast<GLenum>(GL_RGBA12_EXT) || _pixelFormat == static_cast<GLenum>(GL_RGBA16      ) || _pixelFormat == static_cast<GLenum>(GL_RGBA16F         ) ||
					 _pixelFormat == static_cast<GLenum>(GL_RGBA32F   ) || _pixelFormat == static_cast<GLenum>(GL_RGBA32F_ARB ) || _pixelFormat == static_cast<GLenum>(GL_RGBA32F_EXT     )
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
		/**
		 * @brief Binds the provided Texture.
		 *
		 * @param[in] _texture The Texture to bind
		 * @param[in] _force Flag indicating whether the underlying graphics API should bind the Texture regardless of which Texture is currently bound.
		 */
		static void Bind(const Texture& _texture, const bool& _force = false) {
			
			if (_force || Texture::s_CurrentTexture != _texture.m_TextureID) {
				glBindTexture(GL_TEXTURE_2D, s_CurrentTexture = static_cast<GLint>(_texture.m_TextureID));
			}
		}
	
		/**
		 * @brief Binds the provided Texture.
		 *
		 * @param[in] _texture The Texture to bind
		 * @param[in] _force Flag indicating whether the underlying graphics API should bind the Texture regardless of which Texture is currently bound.
		 */
		static void Bind(const GLuint& _texture, const bool& _force = false) {
			
			if (_force || Texture::s_CurrentTexture != _texture) {
				glBindTexture(GL_TEXTURE_2D, s_CurrentTexture = static_cast<GLint>(_texture));
			}
		}
		
		/**
		 * @brief Unbind the currently bound texture.
		 *
		 * @param[in] _texture The Texture object to bind
		 * @param[in] _force Flag indicating whether the underlying graphics API should unbind the Texture regardless of which Texture is currently bound.
		 */
		static void Unbind(const bool& _force = false) {
			
			if (_force || Texture::s_CurrentTexture != GL_NONE) {
				glBindTexture(GL_TEXTURE_2D, s_CurrentTexture = GL_NONE);
			}
		}
		
		/**
		 * @brief Discard the Texture.
		 */
		virtual void Discard() const {
			if (m_TextureID != GL_NONE) {
				Texture::Unbind();
				glDeleteTextures(1, &m_TextureID);
			}
		}
		
		/**
		* @brief Get the Texture's ID.
		*/
		[[nodiscard]] constexpr const GLuint& ID() const noexcept { return m_TextureID; }
		
		explicit constexpr operator GLuint() const noexcept { return this->ID(); }
	
	private:
		
		/** @brief ID of currently bound texture. */
		inline static GLuint s_CurrentTexture { GL_NONE };
		
	protected:
		
		int m_Width,
			m_Height;
		
		GLuint m_TextureID;
		
		Parameters::Format         m_Format; /**< @brief Texture format.      */
		Parameters::FilterMode m_FilterMode; /**< @brief Texture filter mode. */
		Parameters::WrapMode     m_WrapMode; /**< @brief Texture wrap mode.   */
		
		explicit Texture(const int& _width, const int& _height, const GLuint& _textureID, Texture::Parameters::Format  _format, Texture::Parameters::FilterMode  _filterMode, Texture::Parameters::WrapMode  _wrapMode) noexcept :
			m_Width     (_width    ),
			m_Height    (_height   ),
			m_TextureID (_textureID),
			m_Format    (std::move(_format    )),
			m_FilterMode(std::move(_filterMode)),
			m_WrapMode  (std::move(_wrapMode  )) {}
	
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_TEXTURE_HPP