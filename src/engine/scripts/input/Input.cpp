#include "Input.h"

#include "../core/Debug.h"
#include "../core/Time.h"
#include "../core/Window.h"
#include "../ui/GUI.h"

#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_mouse.h>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>

#include <glm/common.hpp>
#include <glm/ext/vector_int2.hpp>

#include <algorithm>
#include <exception>
#include <utility>
#include <vector>

namespace LouiEriksson::Engine::Input {
	
	Input::Key::State::State() noexcept :
			s_Data(nullptr),
			s_Length(0) {}
	
	bool Input::Key::State::Get(const SDL_Keycode& _value) const {
		return Get(SDL_GetScancodeFromKey(_value));
	}
	
	bool Input::Key::State::Get(const SDL_Scancode& _value) const {
		return s_Data == nullptr ?
		       false :
			   s_Data[static_cast<SDL_Scancode>(std::min(static_cast<int>(_value), s_Length))] != 0u;
	}
	
	Hashmap<Uint32, std::vector<SDL_Event>>::optional Input::Event::Get(const Uint32& _event) noexcept {
		return s_Events.Get(_event);
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
	
	const glm::vec2& Input::Mouse::Motion() noexcept {
		return s_Motion;
	}
	
	bool Input::Mouse::Get(const Uint8& _button) noexcept {
		return (s_MouseState & SDL_BUTTON(_button)) != 0u;
	}
	
	bool Input::Mouse::GetDown(const Uint8& _button) {
		
		bool result = false;
		
		if (const auto events = Input::Event::Get(SDL_MOUSEBUTTONDOWN)) {
			
			for (auto item : *events) {
				
				if (item.button.button == _button) {
					
					result = true;
					
					break;
				}
			}
		}
		
		return result;
	}
	
	bool Input::Mouse::GetUp(const Uint8& _button) {
		
		bool result = false;
		
		if (const auto events = Input::Event::Get(SDL_MOUSEBUTTONUP)) {
			
			for (auto item : *events) {
				
				if (item.button.button == _button) {
					
					result = true;
					
					break;
				}
			}
		}
		
		return result;
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
		 * enough as it will fundamentally point to the exact same data.
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
		
		SDL_Event event { 0 };
		
		// Poll for pending events. Calling SDL_PollEvent also updates the internal keyboard state.
		while (SDL_PollEvent(&event) != 0) {
			
			// Send event to GUI for processing.
			UI::GUI::ProcessEvent(event);
			
			// Get list of events of the same type, if they exist.
			auto bucket = Input::Event::s_Events.Get(event.type).value_or({});
			
			// Append the current event to the list.
			bucket.emplace_back(event);
			
			// Update the list in the hashmap.
			Input::Event::s_Events.Emplace(std::move(event.type), std::move(bucket));
		}
		
		/* HANDLE MOUSE STATE */
		{
			glm::ivec2 delta;
			
			Input::Mouse::s_MouseState = SDL_GetRelativeMouseState(&delta.x, &delta.y);
			Input::Mouse::s_Motion = (static_cast<glm::vec2>(delta) / Time::UnscaledDeltaTime<scalar_t>() / static_cast<scalar_t>(1000.0)) * static_cast<scalar_t>(0.1);
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
		
		try {
			
			// Ensure the copied keyboard state is disposed of.
			if (Input::Key::s_PreviousKeyboardState.s_Data != nullptr) {
				delete[] Input::Key::s_PreviousKeyboardState.s_Data;
				
				Input::Key::s_PreviousKeyboardState.s_Data = nullptr;
			}
		}
		catch (const std::exception& e) {
			Debug::Log(e, LogType::Critical);
		}
	}
	
} // LouiEriksson::Engine::Input