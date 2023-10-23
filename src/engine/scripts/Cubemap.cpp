//
// Created by loui on 23/10/23.
//

#include "Cubemap.h"

namespace LouiEriksson {
	
	
	Cubemap::Cubemap(const int& _width, const int& _height) : Texture(_width, _height) {
	
	}
	
	Cubemap::~Cubemap() {
		Discard();
	}
	
	Cubemap Cubemap::Create(const std::vector<Texture>& _textures) {
		return Cubemap(-1, -1);
	}
	
	void Cubemap::Discard() const {
		Texture::Discard();
	}
}