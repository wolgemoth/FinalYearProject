#include "stdafx.h"

#include "RenderTexture.h"

namespace LouiEriksson {
	
	RenderTexture::RenderTexture(const int& _width, const int& _height, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode) : Texture(_width, _height, 0, _format, _filterMode, _wrapMode) {
		
		m_FBO_ID   = -1;
		m_RBO_ID   = -1;
		m_Depth_ID = -1;
		
		Create(_width, _height, _format, _filterMode, _wrapMode);
	}
	
	RenderTexture::~RenderTexture() {
		Discard();
	}
	
	void RenderTexture::Create(const int& _width, const int& _height, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode) {
		
		// Generate the "Frame Buffer Object".
		glGenFramebuffers(1, &m_FBO_ID);
		
		if (m_FBO_ID != 0u) {
			
			RenderTexture::Bind(*this);
			
			// COLOR
			glGenTextures(1, &m_TextureID);
			glBindTexture(GL_TEXTURE_2D, m_TextureID);
			
			glTexImage2D(GL_TEXTURE_2D, 0, _format.PixelFormat(), _width, _height, 0, _format.TextureFormat(), GL_FLOAT, nullptr);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filterMode.Min());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filterMode.Mag());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapMode.WrapS());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapMode.WrapT());
			
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);
			
			// DEPTH
			glGenTextures(1, &m_Depth_ID);
			glBindTexture(GL_TEXTURE_2D, m_Depth_ID);
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filterMode.Min());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filterMode.Mag());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapMode.WrapS());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapMode.WrapT());
			
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Depth_ID, 0);

			// Generate the "Render Buffer Object".
//			glGenRenderbuffers(1, &m_RBO_ID);
//			glBindRenderbuffer(GL_RENDERBUFFER, m_RBO_ID);
//			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
//			glBindRenderbuffer(GL_RENDERBUFFER, 0);
//			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO_ID);

			RenderTexture::Unbind();
			
			m_Width  = _width;
			m_Height = _height;
		}
		else {
			throw std::runtime_error("ERROR (RenderTexture.cpp [RenderTexture(int, int)]): Failure generating Frame Buffer Object for new RenderTexture.");
		}
	}
	
	void RenderTexture::Reinitialise(const int& _width, const int& _height) {
		
		Discard();
		Create(_width, _height, m_Format, m_FilterMode, m_WrapMode);
	}
	
	void RenderTexture::Reinitialise(const int& _width, const int& _height, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode) {
		
		Discard();
		Create(_width, _height, _format, _filterMode, _wrapMode);
	}
	
	void RenderTexture::Bind(const RenderTexture& _rt) {
		glBindFramebuffer(GL_FRAMEBUFFER, _rt.m_FBO_ID);
	}
	
	void RenderTexture::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	void RenderTexture::Discard() const {
		
		if (m_FBO_ID   != -1) { glDeleteFramebuffers (1, &m_FBO_ID); }
		if (m_RBO_ID   != -1) { glDeleteRenderbuffers(1, &m_RBO_ID); }
		if (m_Depth_ID != -1) { glDeleteTextures(1, &m_Depth_ID);    }
		
		Texture::Discard();
	}
	
	GLuint RenderTexture::DepthID() const {
		return m_Depth_ID;
	}
}