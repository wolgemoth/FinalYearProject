#include "stdafx.h"

#include "RenderTexture.h"

namespace LouiEriksson {
	
	RenderTexture::RenderTexture(int _width, int _height) {
	
		glGenFramebuffers(1, &m_FBO_ID);
	
		if (m_FBO_ID) {
	
			Bind();
	
			glGenTextures(1, &m_TextureID);
			glBindTexture(GL_TEXTURE_2D, m_TextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);
	
			glGenRenderbuffers(1, &m_RBO_ID);
			glBindRenderbuffer(GL_RENDERBUFFER, m_RBO_ID);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO_ID);
	
			Unbind();
		}
		else {
			throw std::runtime_error("ERROR (RenderTexture.cpp [RenderTexture(int, int)]): Failure generating Frame Buffer Object for new RenderTexture.");
		}
	}
	
	RenderTexture::~RenderTexture() {
		glDeleteFramebuffers  (1, &m_FBO_ID);
		glDeleteRenderbuffers (1, &m_RBO_ID);
		glDeleteTextures      (1, &m_TextureID);
	}
	
	void RenderTexture::Bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO_ID);
	}
	
	void RenderTexture::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	[[maybe_unused]] GLuint RenderTexture::TextureID() const {
		return m_TextureID;
	}
}