#pragma once

#ifndef FINALYEARPROJECT_RENDER_TEXTURE_H
#define FINALYEARPROJECT_RENDER_TEXTURE_H

#include "Texture.h"

namespace LouiEriksson {
	
	class RenderTexture : public Texture {
	
	private:
		
		GLuint m_FBO_ID,
			   m_RBO_ID;
		
		void Create(const int& _width, const int& _height);
	
	public:
		
		RenderTexture(const int& _width, const int& _height);
		
		~RenderTexture();
		
		RenderTexture(             const RenderTexture& _other) = delete;
		RenderTexture& operator = (const RenderTexture& _other) = delete;
		
		void Resize(const int& _width, const int& _height);
		
		void Discard() const;
		
		static void Bind(const RenderTexture& _rt);
		
		static void Unbind();
	};
	
}

#endif //FINALYEARPROJECT_RENDER_TEXTURE_H