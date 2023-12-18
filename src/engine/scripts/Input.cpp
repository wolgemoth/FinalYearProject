#include "stdafx.h"

#include "Input.h"

#include "Utils.h"

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	std::size_t Input::SDL_Event_Hash::operator ()(const SDL_Event& _item) const {
		return Utils::GetHashcode(_item);
	}
	
	bool Input::SDL_Event_Comparator::operator ()(const SDL_Event& _a, const SDL_Event& _b) const {
		return &_a == &_b;
	}
	
	const Uint8* Input::KeyboardState() {
		return s_KeyboardState;
	}
	
	bool Input::HasEvent(const SDL_Event& _event) {
		return s_Events.find(_event) != s_Events.end();
	}
	
	void Input::Tick() {
		
		s_Events.clear();
		
		SDL_Event event = { 0 };
		
		while (SDL_PollEvent(&event) != 0) {
			
			// Send event to GUI for processing.
			GUI::ProcessEvent(event);
			
			// Add to hash set.
			s_Events.insert(event);
		}
		
		// Assign new keyboard state.
		SDL_PumpEvents();
		s_KeyboardState = SDL_GetKeyboardState(nullptr);
	}
}