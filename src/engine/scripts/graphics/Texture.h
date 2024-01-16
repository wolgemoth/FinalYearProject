#ifndef FINALYEARPROJECT_TEXTURE_H
#define FINALYEARPROJECT_TEXTURE_H

#include <GL/glew.h>

// @Assessor: This class was submitted for GACP. Please don't mark it for GEP.

namespace LouiEriksson::Engine {
	
	class File;
	class Resources;
	class Settings;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Graphics {
	
	class Texture {
		
		friend LouiEriksson::Engine::File;
		friend LouiEriksson::Engine::Resources;
		friend LouiEriksson::Engine::Settings;
	
		friend class Camera;
		friend class Cubemap;
		friend class Light;
		friend class RenderTexture;
		
	public:
	
		struct Parameters {
			
			/// <summary> Container specifying a Texture's format. </summary>
			struct Format {
			
			private:
				GLenum m_PixelFormat;
				GLenum m_TextureFormat;
				
				int m_Channels;
				
				bool m_Mips;
				
			public:
				
				explicit Format(const GLenum& _pixelFormat, const bool& _mips);
				
				Format             (const Format& _other);
				Format& operator = (const Format& _other);
				
				Format             (Format&& _other) noexcept;
				Format& operator = (Format&& _other) noexcept;
				
				[[nodiscard]] const GLenum&   PixelFormat() const noexcept;
				[[nodiscard]] const GLenum& TextureFormat() const noexcept;
				[[nodiscard]] const    int&      Channels() const noexcept;
				[[nodiscard]] const   bool&          Mips() const noexcept;
			};
			
			/// <summary> Container specifying a Texture's filter mode. </summary>
			struct FilterMode {
			
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
			
			/// <summary> Container specifying a Texture's wrap mode. </summary>
			struct WrapMode {
			
			private:
				GLenum m_WrapS;
				GLenum m_WrapT;
				GLenum m_WrapR;
				
			public:
				
				explicit WrapMode(const GLenum& _s, const GLenum& _t = GL_NONE, const GLenum& _r = GL_NONE) noexcept;
				
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
		
		/// <summary> Determine a Texture's format and number of channels using its pixel format. </summary>
		static void GetFormatData(const GLenum& _pixelFormat, GLenum& _textureFormat, int& _channels);
		
		/// <summary> Bind the provided Texture. </summary>
		static void Bind(const Texture& _texture);
		
		/// <summary> Unbind the currently bound Texture. </summary>
		static void Unbind();
		
		/// <summary> Discard the Texture. </summary>
		virtual void Discard() const;
		
		/// <summary> Get the Texture's ID. </summary>
		[[nodiscard]] const GLuint& ID() const noexcept;
		
		explicit operator GLuint() const noexcept { return this->ID(); }
	
	private:
		
		/// <summary> ID of currently bound texture. </summary>
		inline static GLint s_CurrentTexture { GL_NONE };
		
	protected:
		
		int m_Width,
			m_Height;
		
		GLuint m_TextureID;
		
		/// <summary> Texture's formatting parameters. </summary>
		Parameters::Format m_Format;
		
		/// <summary> Texture's filtering parameters. </summary>
		Parameters::FilterMode m_FilterMode;
		
		/// <summary> Texture's wrapping parameters. </sum  mary>
		Parameters::WrapMode m_WrapMode;
		
		explicit Texture(const int& _width, const int& _height, const GLuint& _textureID, Texture::Parameters::Format  _format, Texture::Parameters::FilterMode  _filterMode, Texture::Parameters::WrapMode  _wrapMode) noexcept;
	
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_TEXTURE_H