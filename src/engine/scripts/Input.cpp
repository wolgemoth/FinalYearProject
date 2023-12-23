#include "stdafx.h"

#include "Input.h"

#include "GUI.h"

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	bool Input::Event::Get(const Uint32& _event, std::vector<SDL_Event>& _results) {
		return s_Events.Get(_event, _results);
	}
	
	bool Input::Event::Get(const Uint32& _event) {
		return s_Events.ContainsKey(_event);
	}
	
	bool Input::Key::Get(const SDL_Scancode& _key) {
		return s_KeyboardState[_key] != 0u;
	}
	
	bool Input::Key::Get(const SDL_Keycode& _key) {
		return s_KeyboardState[SDL_GetScancodeFromKey(_key)] != 0u;
	}
	
	glm::vec2 Input::Mouse::Motion() {
		return s_Motion;
	}
	
	void Input::Tick() {
		
		/* HANDLE EVENTS */
		Input::Event::s_Events.Clear(); // Reset collection of events.
		
		SDL_Event event = { 0 };
		
		while (SDL_PollEvent(&event) != 0) {
			
			// Send event to GUI for processing.
			GUI::ProcessEvent(event);
			
			// Get list of events of the same type, if they exist.
			std::vector<SDL_Event> bucket;
			Input::Event::s_Events.Get(event.type, bucket);
			
			// Append the current event to the list.
			bucket.emplace_back(event);
			
			// Update the list in the hashmap.
			Input::Event::s_Events.Assign(event.type, bucket);
		}
		
		/* HANDLE MOUSE INPUT */
		{
			/* MOTION */
			{
				glm::ivec2 motion;
				
				SDL_GetRelativeMouseState(&motion.x, &motion.y);
				
				Input::Mouse::s_Motion = static_cast<glm::vec2>(motion);
			}
		}
		
		/* HANDLE KEYBOARD STATES */
		{
			SDL_PumpEvents();
			Input::Key::s_KeyboardState = SDL_GetKeyboardState(nullptr);
		}
	}
}