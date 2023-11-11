//
// Created by loui on 23/10/23.
//

#ifndef FINALYEARPROJECT_TEXTURE_H
#define FINALYEARPROJECT_TEXTURE_H

#include "stdafx.h"

class Cubemap;

namespace LouiEriksson {
	
	class Texture {
	
		friend class File;
		friend class Cubemap;
		
	protected:
		
		int m_Width,
			m_Height;
		
		GLuint m_TextureID;
		
		explicit Texture(const int& _width, const int& _height, const GLuint& _textureID = -1);
		
		explicit Texture(const GLuint& _textureID);
	
	public:
	
		 Texture();
		~Texture();
		
		Texture             (const Texture&  _other) = delete;
		Texture& operator = (const Texture&  _other) = delete;
		
		Texture             (Texture&& _other) noexcept;
		Texture& operator = (Texture&& _other) noexcept;
		
		[[nodiscard]] const int& Width() const;
		
		[[nodiscard]] const int& Height() const;
		
		void Reinitialise(int _width, int _height);
		
		static void GetFormatData(const GLenum& _internalFormat, GLenum& _textureFormat, int& _channels);
		
		static void Bind(const Texture& _texture);
		
		static void Unbind();
		
		void Discard() const;
		
		[[nodiscard]] const GLuint& ID() const;
		
		explicit operator GLuint() const { return this->ID(); }
	
	};
	
}

#endif //FINALYEARPROJECT_TEXTURE_H