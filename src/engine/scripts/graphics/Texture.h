#ifndef FINALYEARPROJECT_TEXTURE_H
#define FINALYEARPROJECT_TEXTURE_H

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
				
				Format(const GLenum& _pixelFormat, const bool& _mips);
				
				Format             (const Format& _other);
				Format& operator = (const Format& _other);
				
				Format             (Format&& _other) noexcept;
				Format& operator = (Format&& _other) noexcept;
				
				[[nodiscard]] const GLenum&   PixelFormat() const noexcept;
				[[nodiscard]] const GLenum& TextureFormat() const noexcept;
				[[nodiscard]] const    int&      Channels() const noexcept;
				[[nodiscard]] const   bool&          Mips() const noexcept;
			};
			
			/**
			 * @brief Specifies a Texture's filter mode.
			 */
			struct FilterMode final {
			
			private:
				GLenum m_Min;
				GLenum m_Mag;
				
			public:
				
				FilterMode(const GLenum& _min, const GLenum& _mag) noexcept;
				
				FilterMode             (const FilterMode& _other);
				FilterMode& operator = (const FilterMode& _other);
				
				FilterMode             (FilterMode&& _other) noexcept;
				FilterMode& operator = (FilterMode&& _other) noexcept;
				
				[[nodiscard]] const GLenum& Min() const noexcept;
				[[nodiscard]] const GLenum& Mag() const noexcept;
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
				
				WrapMode(const GLenum& _s, const GLenum& _t = GL_NONE, const GLenum& _r = GL_NONE) noexcept;
				
				WrapMode             (const WrapMode& _other);
				WrapMode& operator = (const WrapMode& _other);
				
				WrapMode             (WrapMode&& _other) noexcept;
				WrapMode& operator = (WrapMode&& _other) noexcept;
				
				[[nodiscard]] const GLenum& WrapS() const noexcept;
				[[nodiscard]] const GLenum& WrapT() const noexcept;
				[[nodiscard]] const GLenum& WrapR() const noexcept;
			};
			
		};
		
		~Texture();

		Texture             (const Texture&  _other) = delete;
		Texture& operator = (const Texture&  _other) = delete;

		Texture             (Texture&& _other) noexcept;
		Texture& operator = (Texture&& _other) noexcept;
		
		[[nodiscard]] const int&  Width() const noexcept;
		[[nodiscard]] const int& Height() const noexcept;
		
		[[nodiscard]] const Parameters::Format&         Format() const noexcept;
		[[nodiscard]] const Parameters::FilterMode& FilterMode() const noexcept;
		[[nodiscard]] const Parameters::WrapMode&     WrapMode() const noexcept;
		
		/**
		 * @brief Determine a Texture's format and number of channels using its pixel format
		 *
		 * @param[in] _pixelFormat The pixel format of the texture
		 * @param[out] _textureFormat The texture format (updated by the function)
		 * @param[out] _channels The number of channels (updated by the function)
		 */
		static void GetFormatData(const GLenum& _pixelFormat, GLenum& _textureFormat, int& _channels);
		
		/**
		 * @brief Binds the provided Texture.
		 *
		 * @param[in] _texture The Texture to bind
		 * @param[in] _force Flag indicating whether the underlying graphics API should bind the Texture regardless of which Texture is currently bound.
		 */
		static void Bind(const Texture& _texture, const bool& _force = false);
		/**
		 * @brief Binds the provided Texture.
		 *
		 * @param[in] _texture The Texture to bind
		 * @param[in] _force Flag indicating whether the underlying graphics API should bind the Texture regardless of which Texture is currently bound.
		 */
		static void Bind(const GLuint& _texture, const bool& _force = false);
		
		/**
		 * @brief Unbind the currently bound texture.
		 *
		 * @param[in] _texture The Texture object to bind
		 * @param[in] _force Flag indicating whether the underlying graphics API should unbind the Texture regardless of which Texture is currently bound.
		 */
		static void Unbind(const bool& _force = false);
		
		/**
		 * @brief Discard the Texture.
		 */
		virtual void Discard() const;
		
		/**
		* @brief Get the Texture's ID.
		*/
		[[nodiscard]] const GLuint& ID() const noexcept;
		
		explicit operator GLuint() const noexcept { return this->ID(); }
	
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
		
		explicit Texture(const int& _width, const int& _height, const GLuint& _textureID, Texture::Parameters::Format  _format, Texture::Parameters::FilterMode  _filterMode, Texture::Parameters::WrapMode  _wrapMode) noexcept;
	
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_TEXTURE_H