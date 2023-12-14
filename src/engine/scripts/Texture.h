#pragma once

#ifndef FINALYEARPROJECT_TEXTURE_H
#define FINALYEARPROJECT_TEXTURE_H

#include "stdafx.h"

class Cubemap;

namespace LouiEriksson {
	
	class Texture {
	
		friend class File;
		friend class Cubemap;
		friend class Resources;
		friend class Light;
		friend class Camera;
		friend class Settings;
		friend class RenderTexture;
		
	public:
	
		struct Parameters {
			
			struct Format {
			
			private:
				GLenum m_PixelFormat;
				GLenum m_TextureFormat;
				
				int m_Channels;
				
				bool m_Mips;
				
			public:
				
				explicit Format(const GLenum& _pixelFormat, bool _mips);
				
				Format             (const Format& _other);
				Format& operator = (const Format& _other);
				
				Format             (Format&& _other) noexcept;
				Format& operator = (Format&& _other) noexcept;
				
				[[nodiscard]] const GLenum&   PixelFormat() const;
				[[nodiscard]] const GLenum& TextureFormat() const;
				[[nodiscard]] const    int&      Channels() const;
				[[nodiscard]] const   bool&          Mips() const;
			};
			
			struct FilterMode {
			
			private:
				GLenum m_Min;
				GLenum m_Mag;
				
			public:
				
				FilterMode(const GLenum& _min, const GLenum& _mag);
				
				FilterMode             (const FilterMode& _other);
				FilterMode& operator = (const FilterMode& _other);
				
				FilterMode             (FilterMode&& _other) noexcept;
				FilterMode& operator = (FilterMode&& _other) noexcept;
				
				[[nodiscard]] const GLenum& Min() const;
				[[nodiscard]] const GLenum& Mag() const;
			};
			
			struct WrapMode {
			
			private:
				GLenum m_WrapS;
				GLenum m_WrapT;
				GLenum m_WrapR;
				
			public:
				
				explicit WrapMode(const GLenum& _s, const GLenum& _t = GL_NONE, const GLenum& _r = GL_NONE);
				
				WrapMode             (const WrapMode& _other);
				WrapMode& operator = (const WrapMode& _other);
				
				WrapMode             (WrapMode&& _other) noexcept;
				WrapMode& operator = (WrapMode&& _other) noexcept;
				
				[[nodiscard]] const GLenum& WrapS() const;
				[[nodiscard]] const GLenum& WrapT() const;
				[[nodiscard]] const GLenum& WrapR() const;
			};
		};
		
		~Texture();

		Texture             (const Texture&  _other) = delete;
		Texture& operator = (const Texture&  _other) = delete;

		Texture             (Texture&& _other) noexcept;
		Texture& operator = (Texture&& _other) noexcept;
		
		[[nodiscard]] const int&      Width() const;
		[[nodiscard]] const int&     Height() const;
		
		[[nodiscard]] const Parameters::Format&     Format() const;
		[[nodiscard]] const Parameters::FilterMode& FilterMode() const;
		[[nodiscard]] const Parameters::WrapMode&   WrapMode() const;
		
		static void GetFormatData(const GLenum& _internalFormat, GLenum& _textureFormat, int& _channels);
		
		static void Bind(const Texture& _texture);
		
		static void Unbind();
		
		void Discard() const;
		
		[[nodiscard]] const GLuint& ID() const;
		
		explicit operator GLuint() const { return this->ID(); }
	
	private:
		
		inline static GLint s_CurrentTexture = GL_NONE;
		
	protected:
		
		int m_Width,
			m_Height;
		
		GLuint m_TextureID;
		
		Parameters::Format m_Format;
		
		Parameters::FilterMode m_FilterMode;
		Parameters::  WrapMode m_WrapMode;
		
		explicit Texture(const int& _width, const int& _height, const GLuint& _textureID, Texture::Parameters::Format  _format, Texture::Parameters::FilterMode  _filterMode, Texture::Parameters::WrapMode  _wrapMode);
	};
	
}

#endif //FINALYEARPROJECT_TEXTURE_H