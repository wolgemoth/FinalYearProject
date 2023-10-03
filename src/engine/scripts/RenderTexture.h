#pragma once

#ifndef FINALYEARPROJECT_RENDER_TEXTURE_H
#define FINALYEARPROJECT_RENDER_TEXTURE_H

namespace LouiEriksson {
	
	class RenderTexture {
	
	private:
	
		GLuint m_FBO_ID;
		GLuint m_RBO_ID;
		GLuint m_TextureID;
		
	public:
		
		RenderTexture(const RenderTexture& _other) = delete;
		RenderTexture& operator = (const RenderTexture& _other) = delete;
		
		RenderTexture(int _width, int _height);
	
		~RenderTexture();
	
		void Bind() const;
	
		static void Unbind();
		
		[[nodiscard]] GLuint TextureID() const;
	};
}

#endif //FINALYEARPROJECT_RENDER_TEXTURE_H