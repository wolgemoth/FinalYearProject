#include "RenderTexture.h"

#include "../../core/Debug.h"
#include "../Texture.h"

#include <GL/glew.h>

#include <stdexcept>
#include <string>

namespace LouiEriksson::Engine::Graphics {
	
	RenderTexture::RenderTexture(const int& _width, const int& _height, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode, const RenderTexture::Parameters::DepthMode& _depthMode) : Texture(_width, _height, 0, _format, _filterMode, _wrapMode),
			m_FBO_ID   (GL_NONE),
			m_RBO_ID   (GL_NONE),
			m_Depth_ID (GL_NONE),
			m_DepthMode(RenderTexture::Parameters::DepthMode::NONE)
	{
		Create(_width, _height, _format, _filterMode, _wrapMode, _depthMode);
	}
	
	RenderTexture::~RenderTexture() {
		Discard();
	}
	
	void RenderTexture::Create(const int& _width, const int& _height, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode, const RenderTexture::Parameters::DepthMode& _depthMode) {
		
		// Generate the "Frame Buffer Object".
		glGenFramebuffers(1, &m_FBO_ID);
		
		if (m_FBO_ID != GL_NONE) {
			
			Bind(*this);
			
			// COLOR
			glGenTextures(1, &m_TextureID);
			glBindTexture(GL_TEXTURE_2D, Texture::s_CurrentTexture = static_cast<GLint>(m_TextureID));
			
			glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(_format.PixelFormat()), _width, _height, 0, _format.TextureFormat(), GL_HALF_FLOAT, nullptr);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(_filterMode.Min()));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(_filterMode.Mag()));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(_wrapMode.WrapS()));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(_wrapMode.WrapT()));
			
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);
			
			// DEPTH
			switch (_depthMode) {
				case RenderTexture::Parameters::NONE: {
					break;
				}
				case RenderTexture::Parameters::RENDER_BUFFER: {
					
					glGenRenderbuffers(1, &m_RBO_ID);
					glBindRenderbuffer(GL_RENDERBUFFER, m_RBO_ID);
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
					glBindRenderbuffer(GL_RENDERBUFFER, 0);
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO_ID);
					
					break;
				}
				case RenderTexture::Parameters::FRAME_BUFFER: {
					
					glGenTextures(1, &m_Depth_ID);
					glBindTexture(GL_TEXTURE_2D, Texture::s_CurrentTexture = static_cast<GLint>(m_Depth_ID));
					
					// TODO: Investigate compatibility of GL_HALF_FLOAT vs GL_FLOAT on non-Nvidia cards.
					glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_HALF_FLOAT, nullptr);
					
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(_filterMode.Min()));
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(_filterMode.Mag()));
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(_wrapMode.WrapS()));
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(_wrapMode.WrapT()));
					
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Depth_ID, 0);
					
					break;
				}
				default: {
					Debug::Log("Unknown Depth Mode \"" + std::to_string(m_DepthMode) + "\".", LogType::Error);

					break;
				}
			}
			
			m_Width     = _width;
			m_Height    = _height;
			m_DepthMode = _depthMode;
		}
		else {
			Debug::Log("Failed generating Frame Buffer Object for new RenderTexture!", LogType::Error);
		}
	}
	
	void RenderTexture::Reinitialise(const int& _width, const int& _height) {
		
		Discard();
		Create(_width, _height, m_Format, m_FilterMode, m_WrapMode, m_DepthMode);
	}
	
	void RenderTexture::Reinitialise(const int& _width, const int& _height, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode, const RenderTexture::Parameters::DepthMode& _depthMode) {
		
		Discard();
		Create(_width, _height, _format, _filterMode, _wrapMode, _depthMode);
	}
	
	void RenderTexture::Bind(const RenderTexture& _rt) {
		Bind(_rt.m_FBO_ID);
	}
	
	void RenderTexture::Bind(const GLuint& _fbo) {
		
		if (s_CurrentFBO != _fbo) {
			glBindFramebuffer(GL_FRAMEBUFFER, s_CurrentFBO = _fbo);
		}
	}
	
	void RenderTexture::Unbind() {
		
		if (s_CurrentFBO != GL_NONE) {
			glBindFramebuffer(GL_FRAMEBUFFER, s_CurrentFBO = GL_NONE);
		}
	}
	
	void RenderTexture::Discard() const {
		
		if (m_FBO_ID != GL_NONE) {
			RenderTexture::Unbind();
			glDeleteFramebuffers (1, &m_FBO_ID);
		}
		
		if (m_RBO_ID != GL_NONE) {
			glBindRenderbuffer(GL_RENDERBUFFER, GL_NONE);
			glDeleteRenderbuffers(1, &m_RBO_ID);
		}
		
		if (m_Depth_ID != GL_NONE) {
			Texture::Unbind();
			glDeleteTextures(1, &m_Depth_ID);
		}
		
		Texture::Discard();
	}
	
	GLuint RenderTexture::DepthID() const noexcept {
		return m_Depth_ID;
	}
	
} // LouiEriksson::Engine::Graphics