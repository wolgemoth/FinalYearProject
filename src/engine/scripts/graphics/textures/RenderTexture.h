#ifndef FINALYEARPROJECT_RENDER_TEXTURE_H
#define FINALYEARPROJECT_RENDER_TEXTURE_H

#include "../Texture.h"

#include <GL/glew.h>

// @Assessor: This class was submitted for GACP. Please don't mark it for 3DGP.

namespace LouiEriksson {
	
	class RenderTexture : public Texture {
	
		friend class Light;
		friend class Camera;
		
	public:
		
		struct Parameters {
		
			enum DepthMode : char {
				NONE,
				RENDER_BUFFER,
				 FRAME_BUFFER
			};
			
		};
		
		RenderTexture(const int& _width, const int& _height, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode, const RenderTexture::Parameters::DepthMode& _depthMode);
		
		~RenderTexture();
		
		RenderTexture(             const RenderTexture& _other) = delete;
		RenderTexture& operator = (const RenderTexture& _other) = delete;
		
		/// <summary> Reinitialise the texture with a given width and height. </summary>
		void Reinitialise(const int& _width, const int& _height);
		
		/// <summary> Reinitialise the texture with a given width, height, format, filter mode, wrap mode, and depth attachment mode. </summary>
		void Reinitialise(const int& _width, const int& _height, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode, const RenderTexture::Parameters::DepthMode& _depthMode);
		
		/// <summary> Bind the provided RenderTexture. </summary>
		static void Bind(const RenderTexture& _rt);
		
		/// <summary> Unbind the currently bound RenderTexture. </summary>
		static void Unbind();
		
		/// <summary> Discard the RenderTexture. </summary>
		void Discard() const override;
		
		/// <summary> Get the ID of the depth attachment. </summary>
		[[nodiscard]] GLuint DepthID() const;
	
	private:
		
		/// <summary> Currently bound FBO. </summary>
		inline static GLuint s_CurrentFBO { GL_NONE };
		
		GLuint m_FBO_ID,
			   m_RBO_ID,
			   m_Depth_ID;
		
		/// <summary> RenderTexture's depth attachment mode. </summary>
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