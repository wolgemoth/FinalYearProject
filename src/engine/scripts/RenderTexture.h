#pragma once

#ifndef FINALYEARPROJECT_RENDER_TEXTURE_H
#define FINALYEARPROJECT_RENDER_TEXTURE_H

namespace LouiEriksson {
	
	class RenderTexture {
	
	private:
		
		int m_Width,
			m_Height;
		
		GLuint m_FBO_ID,
			   m_RBO_ID,
			   m_TextureID;
		
		void Create(const int& _width, const int& _height);
	
	public:
		
		RenderTexture(const int& _width, const int& _height);
		
		~RenderTexture();
		
		RenderTexture(             const RenderTexture& _other) = delete;
		RenderTexture& operator = (const RenderTexture& _other) = delete;
		
		[[nodiscard]] const int& Width()  const;
		[[nodiscard]] const int& Height() const;
		
		void Resize(const int& _width, const int& _height);
		
		static void Bind(const RenderTexture& _rt);
		
		static void Unbind();
		
		void Discard() const;
		
		[[nodiscard]] const GLuint& ID() const;
		
		explicit operator GLuint() const { return this->ID(); }
	};
	
}

#endif //FINALYEARPROJECT_RENDER_TEXTURE_H