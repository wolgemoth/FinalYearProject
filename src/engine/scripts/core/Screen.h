#ifndef FINALYEARPROJECT_SCREEN_H
#define FINALYEARPROJECT_SCREEN_H

#include <glm/ext/vector_int2.hpp>

namespace LouiEriksson::Engine {
	
	class Screen final {
	
	public:
		
		static glm::ivec2 Dimensions(const int& _screenID);
		
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_SCREEN_H