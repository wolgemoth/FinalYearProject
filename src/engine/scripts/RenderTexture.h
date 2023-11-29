#pragma once

#ifndef FINALYEARPROJECT_RENDER_TEXTURE_H
#define FINALYEARPROJECT_RENDER_TEXTURE_H

#include "Texture.h"

namespace LouiEriksson {
	
	class RenderTexture : public Texture {
	
		friend class Light;
		friend class Camera;
		
	public:
		
		struct Parameters {
		
			enum DepthMode {
				NONE,
				RENDER_BUFFER,
				 FRAME_BUFFER
			};
			
		};
		
		RenderTexture(const int& _width, const int& _height, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode, const RenderTexture::Parameters::DepthMode& _depthMode);
		
		~RenderTexture();
		
		RenderTexture(             const RenderTexture& _other) = delete;
		RenderTexture& operator = (const RenderTexture& _other) = delete;
		
		void Reinitialise(const int& _width, const int& _height);
		
		void Reinitialise(const int& _width, const int& _height, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode, const RenderTexture::Parameters::DepthMode& _depthMode);
		
		static void Bind(const RenderTexture& _rt);
		
		static void Unbind();
		
		void Discard() const;
		
		GLuint DepthID() const;
	
	private:
		
		inline static GLuint s_CurrentFBO = GL_NONE;
		
		GLuint m_FBO_ID,
			   m_RBO_ID,
			   m_Depth_ID;
		
		Parameters::DepthMode m_DepthMode;
		
		void Create(
				const int& _width, const int& _height,
				const Texture::Parameters::Format&             _format,
				const Texture::Parameters::FilterMode&     _filterMode,
				const Texture::Parameters::WrapMode&         _wrapMode,
				const RenderTexture::Parameters::DepthMode& _depthMode
		);
	};
}

#endif //FINALYEARPROJECT_RENDER_TEXTURE_H