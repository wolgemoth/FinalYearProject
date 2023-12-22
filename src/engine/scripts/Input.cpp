#include "stdafx.h"

#include "Input.h"

#include "GUI.h"

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	const Uint8* Input::KeyboardState() {
		return s_KeyboardState;
	}
	
	bool Input::Get(const Uint32& _event, std::vector<SDL_Event>& _results) {
		return s_Events.Get(_event, _results);
	}
	
	bool Input::Get(const Uint32& _event) {
		return s_Events.ContainsKey(_event);
	}
	
	void Input::Tick() {
		
		s_Events.Clear(); // Reset collection of events.
		
		SDL_Event event = { 0 };
		
		while (SDL_PollEvent(&event) != 0) {
			
			// Send event to GUI for processing.
			GUI::ProcessEvent(event);
			
			// Get list of events of the same type, if they exist.
			std::vector<SDL_Event> bucket;
			s_Events.Get(event.type, bucket);
			
			// Append the current event to the list.
			bucket.emplace_back(event);
			
			// Update the list in the hashmap.
			s_Events.Assign(event.type, bucket);
		}
		
		// Assign new keyboard state.
		SDL_PumpEvents();
		s_KeyboardState = SDL_GetKeyboardState(nullptr);
	}
}