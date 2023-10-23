//
// Created by loui on 23/10/23.
//

#ifndef FINALYEARPROJECT_TEXTURE_H
#define FINALYEARPROJECT_TEXTURE_H

namespace LouiEriksson {
	
	class Texture {
		
	protected:
		
		int m_Width,
			m_Height;
		
		GLuint m_TextureID;
	
		explicit Texture(const int& _width, const int& _height);
		
	public:
	
		static Texture Create(std::string& _path);
		
		explicit Texture(const GLuint& _textureID);
		
		~Texture();
		
		Texture             (const Texture& _other) = delete;
		Texture& operator = (const Texture& _other) = delete;
		
		[[nodiscard]] const int& Width() const;
		
		[[nodiscard]] const int& Height() const;
		
		static void Bind(const Texture& _texture);
		
		static void Unbind();
		
		void Discard() const;
		
		[[nodiscard]] const GLuint& ID() const;
		
		explicit operator GLuint() const { return this->ID(); }
	
	};
	
}

#endif //FINALYEARPROJECT_TEXTURE_H