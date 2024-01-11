#include "Screen.h"

namespace LouiEriksson {
	
	glm::ivec2 Screen::Dimensions(const int& _screenID) {
		
		SDL_Rect screen;
		SDL_GetDisplayBounds(_screenID, &screen);
		
		return { screen.w, screen.h };
	}
} // LouiEriksson