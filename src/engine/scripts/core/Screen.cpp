#include "Screen.h"

#include <glm/ext/vector_int2.hpp>
#include <SDL_rect.h>
#include <SDL_video.h>

namespace LouiEriksson {
	
	glm::ivec2 Screen::Dimensions(const int& _screenID) {
		
		SDL_Rect screen;
		SDL_GetDisplayBounds(_screenID, &screen);
		
		return { screen.w, screen.h };
	}
} // LouiEriksson