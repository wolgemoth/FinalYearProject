#include "stdafx.h"

#include "RenderTexture.h"

namespace LouiEriksson {
	
	RenderTexture::RenderTexture(const int& _width, const int& _height) {
		
		m_Width  = -1;
		m_Height = -1;
		
		   m_FBO_ID = -1;
		   m_RBO_ID = -1;
		m_TextureID = -1;
		
		Create(_width, _height);
	}
	
	RenderTexture::~RenderTexture() {
		Discard();
	}
	
	void RenderTexture::Create(const int& _width, const int& _height) {
		
		// Generate the "Frame Buffer Object".
		glGenFramebuffers(1, &m_FBO_ID);
		
		if (m_FBO_ID) {
			
			Bind();
			
			// Generate the texture.
			glGenTextures(1, &m_TextureID);
			glBindTexture(GL_TEXTURE_2D, m_TextureID);
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _width, _height, 0, GL_RGBA, GL_FLOAT, NULL);
			
			// Bilinear color filtering.
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			// Clamp to edges of texture.
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);
			
			// Generate the "Render Buffer Object".
			glGenRenderbuffers(1, &m_RBO_ID);
			glBindRenderbuffer(GL_RENDERBUFFER, m_RBO_ID);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO_ID);
			
			Unbind();
			
			m_Width  = _width;
			m_Height = _height;
		}
		else {
			throw std::runtime_error("ERROR (RenderTexture.cpp [RenderTexture(int, int)]): Failure generating Frame Buffer Object for new RenderTexture.");
		}
	}
	
	const int& RenderTexture::Width()  const { return m_Width;  }
	const int& RenderTexture::Height() const { return m_Height; }
	
	void RenderTexture::Resize(const int& _width, const int& _height) {
		
		Discard();
		Create(_width, _height);
	}
	
	void RenderTexture::Bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO_ID);
	}
	
	void RenderTexture::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	void RenderTexture::Discard() const {
		glDeleteFramebuffers(1, &m_FBO_ID);
		glDeleteRenderbuffers(1, &m_RBO_ID);
		glDeleteTextures(1, &m_TextureID);
	}
	
	const GLuint& RenderTexture::ID() const {
		return m_TextureID;
	}
}