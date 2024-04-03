#ifndef FINALYEARPROJECT_RENDER_TEXTURE_H
#define FINALYEARPROJECT_RENDER_TEXTURE_H

#include "../Texture.h"

#include <GL/glew.h>

namespace LouiEriksson::Engine::Graphics {
	
	class RenderTexture final : public Texture {
		
		friend class Camera;
		friend class Light;
		
	public:
		
		struct Parameters {
		
			enum DepthMode : char {
				
				/// <summary> No depth attachment will be generated. </summary>
				NONE,
				
				/// <summary> A render buffer will be generated as a depth attachment. </summary>
				RENDER_BUFFER,
				
				/// <summary> A frame buffer will be generated as a depth attachment. </summary>
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
		
		/// <summary> Make the RenderTexture share the depth attachment of another RenderTexture. </summary>
		void ShareDepthAttachment(const RenderTexture& _other);
		
		/// <summary> Get the ID of the depth attachment. </summary>
		[[nodiscard]] GLuint DepthID() const noexcept;
	
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
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_RENDER_TEXTURE_H