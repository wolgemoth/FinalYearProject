#pragma once

#ifndef FINALYEARPROJECT_RENDER_TEXTURE_H
#define FINALYEARPROJECT_RENDER_TEXTURE_H

#include "Texture.h"

namespace LouiEriksson {
	
	class RenderTexture : public Texture {
	
	private:
		
		GLuint m_FBO_ID,
			   m_RBO_ID,
			   m_Depth_ID;
		
		void Create(
				const int& _width, const int& _height,
				const Texture::Parameters::Format& _format,
				const Texture::Parameters::FilterMode& _filterMode,
				const Texture::Parameters::WrapMode& _wrapMode
		);
	
	public:
		
		RenderTexture(const int& _width, const int& _height, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode);
		
		~RenderTexture();
		
		RenderTexture(             const RenderTexture& _other) = delete;
		RenderTexture& operator = (const RenderTexture& _other) = delete;
		
		void Reinitialise(const int& _width, const int& _height);
		
		void Reinitialise(const int& _width, const int& _height, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode);
		
		static void Bind(const RenderTexture& _rt);
		
		static void Unbind();
		
		void Discard() const;
		
		GLuint DepthID() const;
	};
	
}

#endif //FINALYEARPROJECT_RENDER_TEXTURE_H