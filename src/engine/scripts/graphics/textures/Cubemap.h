#ifndef FINALYEARPROJECT_CUBEMAP_H
#define FINALYEARPROJECT_CUBEMAP_H

#include "../Texture.h"

#include <GL/glew.h>

namespace LouiEriksson::Engine {
	
	class File;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Graphics {
	
	class Cubemap final : public Texture {
	
		friend LouiEriksson::Engine::File;
		
		friend class Camera;
		
	private:
		
		Cubemap(const int& _width, const int& _height, const GLuint& _textureID, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode);
		
	public:
		
		~Cubemap();
		
		Cubemap(             const Cubemap& _other) = delete;
		Cubemap& operator = (const Cubemap& _other) = delete;
		
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_CUBEMAP_H