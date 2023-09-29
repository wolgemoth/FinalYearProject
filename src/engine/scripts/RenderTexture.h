#pragma once

#ifndef _RENDER_TEXTURE_H
#define _RENDER_TEXTURE_H

class RenderTexture {

private:

	GLuint m_FBO_ID;
	GLuint m_RBO_ID;
	GLuint m_TextureID;

	RenderTexture(const RenderTexture& _other) = delete; 
	RenderTexture& operator = (const RenderTexture& _other) = delete;

public:

	RenderTexture(int _width, int _height);

	~RenderTexture();

	void Bind();

	void Unbind();

	GLuint TextureID();

};

#endif // !_RENDER_TEXTURE_H_
