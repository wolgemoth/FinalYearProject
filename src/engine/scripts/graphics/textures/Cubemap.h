#ifndef FINALYEARPROJECT_CUBEMAP_H
#define FINALYEARPROJECT_CUBEMAP_H

#include "../Texture.h"
#include "GL/glew.h"

// @Assessor: This class was submitted for GACP. Please don't mark it for GEP.

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
	};
	
}

#endif //FINALYEARPROJECT_CUBEMAP_H