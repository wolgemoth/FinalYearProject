//
// Created by loui on 23/10/23.
//

#ifndef FINALYEARPROJECT_CUBEMAP_H
#define FINALYEARPROJECT_CUBEMAP_H

#include "Texture.h"

namespace LouiEriksson {
	
	class Cubemap : public Texture {
	
	private:
		
		Cubemap(const int& _width, const int& _height);
		
	public:
		
		static Cubemap Create(const std::vector<Texture>& _textures);
		
		~Cubemap();
		
		Cubemap(            const Cubemap& _other) = delete;
		Cubemap& operator =(const Cubemap& _other) = delete;
		
		void Discard() const;
	};
	
}

#endif //FINALYEARPROJECT_CUBEMAP_H