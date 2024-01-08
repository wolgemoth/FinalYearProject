#include "stdafx.h"

#include "Input.h"

#include "GUI.h"

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	Input::Key::State::State() {
		s_Data = nullptr;
		s_Length = 0;
	}
	
	Input::Key::State::~State() {}
	
	bool Input::Key::State::Get(const SDL_Keycode& _value) const {
		return Get(SDL_GetScancodeFromKey(_value));
	}
	
	bool Input::Key::State::Get(const SDL_Scancode& _value) const {
		return s_Data == nullptr ?
		       false :
			   s_Data[static_cast<SDL_Scancode>(glm::min(static_cast<int>(_value), s_Length))] != 0u;
	}
	
	bool Input::Event::Get(const Uint32& _event, std::vector<SDL_Event>& _results) {
		return s_Events.Get(_event, _results);
	}
	
	bool Input::Event::Get(const Uint32& _event) {
		return s_Events.ContainsKey(_event);
	}
	
	bool Input::Key::Get(const SDL_Scancode& _key) {
		return s_CurrentKeyboardState.Get(_key);
	}
	
	bool Input::Key::Get(const SDL_Keycode& _key) {
		return s_CurrentKeyboardState.Get(_key);
	}
	
	bool Input::Key::GetDown(const SDL_Scancode& _key) {
		
		const auto previousState = s_PreviousKeyboardState.Get(_key);
		const auto  currentState =  s_CurrentKeyboardState.Get(_key);
		
		return !previousState && currentState;
	}
	
	bool Input::Key::GetDown(const SDL_Keycode& _key) {
		
		const auto previousState = s_PreviousKeyboardState.Get(_key);
		const auto  currentState =  s_CurrentKeyboardState.Get(_key);
		
		return !previousState && currentState;
	}
	
	bool Input::Key::GetUp(const SDL_Scancode& _key) {
		
		const auto previousState = s_PreviousKeyboardState.Get(_key);
		const auto  currentState =  s_CurrentKeyboardState.Get(_key);
		
		return previousState && !currentState;
	}
	
	bool Input::Key::GetUp(const SDL_Keycode& _key) {
		
		const auto previousState = s_PreviousKeyboardState.Get(_key);
		const auto  currentState =  s_CurrentKeyboardState.Get(_key);
		
		return previousState && !currentState;
	}
	
	const glm::vec2& Input::Mouse::Motion() {
		return s_Motion;
	}
	
	void Input::Tick() {
		
		/*
		 * CACHE PREVIOUS KEYBOARD STATE
		 *
		 * Make a copy of the current keyboard state and save it as the last,
		 * then update the current keyboard state.
		 *
		 * This takes some finangling as the keyboard state (as represented by
		 * SDL2) is a pointer to an internal SDL2 array. This means that simply
		 * having a second copy of the pointer to the internal array isn't
		 * enough as it will fundamentally point to the same data as all others.
		 *
		 * To remedy this, we manually copy the internal SDL2 array and use that
		 * for our previous keyboard state. We also wrap the state into a struct
		 * so we can better organise the code.
		 */
		
		// Skip memory allocation if the reference data is nullptr.
		if (Input::Key::s_CurrentKeyboardState.s_Data != nullptr) {
		
			// Only allocate memory if the copy is nullptr.
			if (Input::Key::s_PreviousKeyboardState.s_Data == nullptr) {
				
				// Allocate a fresh buffer for the copied data.
				Input::Key::s_PreviousKeyboardState.s_Data = new Uint8[Input::Key::s_CurrentKeyboardState.s_Length];
			
				// Copy over the length.
				Input::Key::s_PreviousKeyboardState.s_Length = Input::Key::s_CurrentKeyboardState.s_Length;
			}
			
			// Update the copied data with a clone the reference data:
			std::copy(
				Input::Key::s_CurrentKeyboardState.s_Data,
				Input::Key::s_CurrentKeyboardState.s_Data + Input::Key::s_CurrentKeyboardState.s_Length,
				const_cast<Uint8*>(Input::Key::s_PreviousKeyboardState.s_Data)
			);
		}
		
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
			// Update SDL2's internal keyboard state.
			SDL_PumpEvents();
			
			/*
			 * SDL2's documentation reports that the pointer to their internal
			 * keyboard state never changes. The length of their internal keyboard
			 * state probably also never changes. Therefore, copying the same
			 * pointer each frame to 'update' the keyboard is probably pointless,
			 * and can be done once as a process of initialisation.
			 */
			if (Input::Key::s_CurrentKeyboardState.s_Data == nullptr) {
				
				// Get the SDL2 keyboard state pointer and length of underlying state:
				Input::Key::s_CurrentKeyboardState.s_Data = SDL_GetKeyboardState(&Input::Key::s_CurrentKeyboardState.s_Length);
			}
		}
	}
	
	void Input::Dispose() {
		
		// Ensure the copied keyboard state is disposed of.
		if (Input::Key::s_PreviousKeyboardState.s_Data != nullptr) {
			delete Input::Key::s_PreviousKeyboardState.s_Data;
			
			Input::Key::s_PreviousKeyboardState.s_Data = nullptr;
		}
	}
}