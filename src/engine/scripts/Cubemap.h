//
// Created by loui on 23/10/23.
//

#ifndef FINALYEARPROJECT_CUBEMAP_H
#define FINALYEARPROJECT_CUBEMAP_H

#include "Texture.h"

namespace LouiEriksson {
	
	class Cubemap : public Texture {
	
		friend class Camera;
		friend class File;
		
	private:
		
		Cubemap(const int& _width, const int& _height, const GLuint& _textureID, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode);
		
	public:
		
		~Cubemap();
		
		Cubemap(             const Cubemap& _other) = delete;
		Cubemap& operator = (const Cubemap& _other) = delete;
		
		static void Bind(const Cubemap& _cubemap);
		
		static void Unbind();
		
		void Discard() const;
	};
	
}

#endif //FINALYEARPROJECT_CUBEMAP_H