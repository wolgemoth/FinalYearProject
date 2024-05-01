#ifndef FINALYEARPROJECT_INPUT_HPP
#define FINALYEARPROJECT_INPUT_HPP

#include "../core/Debug.hpp"
#include "../core/Time.hpp"
#include "../core/utils/Hashmap.hpp"
#include "../ui/GUI.hpp"

#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_mouse.h>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>

#include <glm/ext/vector_int2.hpp>

#include <algorithm>
#include <exception>
#include <utility>
#include <vector>

namespace LouiEriksson::Engine {
	
	class Application;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Input {
	
	/**
	 * @class Input
	 * @brief Provides an interface for accessing and managing device input.
	 */
	class Input final {
	
		friend LouiEriksson::Engine::Application;
	
	private:
	
	public:
		
		/**
		 * @class Event
		 * @brief Provides an interface for detecting SDL_Events.
		 */
		class Event final {
		
			friend Input;
		
		private:
		
			/** @brief Hashmap of all SDL_Events for this frame. */
			inline static Hashmap<Uint32, std::vector<SDL_Event>> s_Events;

		public:
		
			/** @brief Get all events of a certain type which occurred this frame. */
			static Hashmap<Uint32, std::vector<SDL_Event>>::optional_ref Get(const Uint32& _event) noexcept {
				return s_Events.Get(_event);
			}
			
		};
		
		/**
		 * @class Key
		 * @brief Provides an interface for detecting key presses.
		 */
		class Key final {
		
			friend Input;
		
		private:
			
			/**
			 * @class State
			 * @brief Wraps a c-style pointer representing the state of the keyboard.
			 */
			struct State final {
			
				friend Input;
				
			private:
				
				const Uint8* s_Data;
				int s_Length;
				
			public:
				
				 State() noexcept :
					s_Data(nullptr),
					s_Length(0) {}
				
				/** @brief  Returns the state of the keyboard at the provided SDL_KeyCode. */
				[[nodiscard]] bool Get(const SDL_Keycode& _value) const {
					return Get(SDL_GetScancodeFromKey(_value));
				}
				
				/** @brief Returns the state of the keyboard at the provided SDL_Scancode. */
				[[nodiscard]] bool Get(const SDL_Scancode& _value) const {
					return s_Data == nullptr ?
					       false :
						   s_Data[static_cast<SDL_Scancode>(std::min(static_cast<const int>(_value), s_Length))] != 0U;
				}
				
			};
			
			inline static State s_PreviousKeyboardState{}; /**< @brief Keyboard state previous tick. */
			inline static State  s_CurrentKeyboardState{}; /**< @brief Keyboard state current tick. */
		
		public:
			
			/**
			 * @brief Returns true if the provided key (in the form of SDL_Scancode) is held.
			 *
			 * This function checks if the provided key is currently being held.
			 *
			 * @param[in] _key The SDL_Scancode of the key to check.
			 * @return True if the key is pressed, false otherwise.
			 */
			static bool Get(const SDL_Scancode& _key) {
				return s_CurrentKeyboardState.Get(_key);
			}
			
			/**
			 * @brief Returns true if the provided key (in the form of SDL_Keycode) is held.
			 *
			 * This function checks if the provided key is currently being held.
			 *
			 * @param[in] _key The SDL_Keycode of the key to check.
			 * @return True if the key is pressed, false otherwise.
			 */
			 static bool Get(const SDL_Keycode& _key) {
				return s_CurrentKeyboardState.Get(_key);
			}
			
			/**
			 * @brief Returns true if the provided key (in the form of SDL_Scancode) was pressed.
			 *
			 * This function checks if the provided key was released.
			 *
			 * @param[in] _key The SDL_Scancode of the key to check.
			 * @return True if the key is pressed, false otherwise.
			 */
			static bool GetDown(const SDL_Scancode& _key) {
		
				const auto previousState = s_PreviousKeyboardState.Get(_key);
				const auto  currentState =  s_CurrentKeyboardState.Get(_key);
				
				return !previousState && currentState;
			}
			
			/**
			 * @brief Returns true if the provided key (in the form of SDL_Keycode) was pressed.
			 *
			 * This function checks if the provided key was released.
			 *
			 * @param[in] _key The SDL_Keycode of the key to check.
			 * @return True if the key is pressed, false otherwise.
			 */
			static bool GetDown(const SDL_Keycode& _key) {
				
				const auto previousState = s_PreviousKeyboardState.Get(_key);
				const auto  currentState =  s_CurrentKeyboardState.Get(_key);
				
				return !previousState && currentState;
			}
			
			/**
			 * @brief Returns true if the provided key (in the form of SDL_Scancode) was released.
			 *
			 * This function checks if the provided key was previously pressed and is now released.
			 *
			 * @param[in] _key The SDL_Scancode of the key to check.
			 * @return True if the key is released, false otherwise.
			 */
			static bool GetUp(const SDL_Scancode& _key) {
				
				const auto previousState = s_PreviousKeyboardState.Get(_key);
				const auto  currentState =  s_CurrentKeyboardState.Get(_key);
				
				return previousState && !currentState;
			}
			
			/**
			 * @brief Returns true if the provided key (in the form of SDL_Keycode) was released.
			 *
			 * This function checks if the provided key was previously pressed and is now released.
			 *
			 * @param[in] _key The SDL_Keycode of the key to check.
			 * @return True if the key is released, false otherwise.
			 */
			static bool GetUp(const SDL_Keycode& _key) {
				
				const auto previousState = s_PreviousKeyboardState.Get(_key);
				const auto  currentState =  s_CurrentKeyboardState.Get(_key);
				
				return previousState && !currentState;
			}
			
		};
		
		/**
		 * @class Mouse
		 * @brief Provides an interface for detecting mouse input.
		 */
		struct Mouse final {
		
			friend Input;
			friend Application;
		
		private:
			
			/** @brief SDL mouse relative state.*/
			inline static Uint32 s_MouseState { 0U };
			
			/** @brief Pixels the mouse has moved since previous tick. */
			inline static vec2 s_Motion { 0.0, 0.0 };
			
		public:
			
			/**
			 * @fn const vec2& Input::Mouse::Motion() noexcept
			 * @brief Retrieves the motion of the mouse.
			 * @return The pixels the mouse has moved since the previous frame.
			 */
			static constexpr const vec2& Motion() noexcept {
				return s_Motion;
			}
			
			/**
			 * @fn static bool Input::Mouse::Get(const Uint8& _button) noexcept
			 * @brief Returns true if the provided mouse button is held.
			 *
			 * This function checks if the specified mouse button is currently being held down.
			 * The mouse button is represented by an SDL button constant, such as SDL_BUTTON_LEFT.
			 *
			 * @param[in] _button The mouse button to check.
			 * @return true if the mouse button is currently held down, false otherwise.
			 */
			static bool Get(const Uint8& _button) noexcept {
				return (s_MouseState & static_cast<unsigned>(SDL_BUTTON(_button))) != 0U;
			}
			
			/**
			 * @fn static bool Input::Mouse::GetDown(const Uint8& _button)
			 * @brief Returns true if the provided mouse button was pressed.
			 *
			 * This function checks if the specified mouse button was pressed.
			 * The mouse button is represented by an SDL button constant, such as SDL_BUTTON_LEFT.
			 *
			 * @param[in] _button The mouse button to check.
			 * @return true if the mouse button was pressed, false otherwise.
			 */
			static bool GetDown(const Uint8& _button) {
				
				bool result = false;
				
				if (const auto events = Input::Event::Get(SDL_MOUSEBUTTONDOWN)) {
					
					for (const auto& item : *events) {
						
						if (item.button.button == _button) {
							result = true;
							break;
						}
					}
				}
				
				return result;
			}
			
			/**
			 * @fn bool Mouse::GetUp(const Uint8& _button)
			 * @brief Returns true if the provided mouse button was released.
			 *
			 * This function checks if the specified mouse button was released.
			 * The mouse button is represented by an SDL button constant, such as SDL_BUTTON_LEFT.
			 *
			 * @param[in] _button The mouse button to check.
			 * @return true if the mouse button was released, false otherwise.
			 */
			static bool GetUp(const Uint8& _button) {
		
				bool result = false;
				
				if (const auto events = Input::Event::Get(SDL_MOUSEBUTTONUP)) {
					
					for (const auto& item : *events) {
						
						if (item.button.button == _button) {
							result = true;
							break;
						}
					}
				}
				
				return result;
			}
			
		};
		
		 Input()                    = delete;
		 Input(const Input& _other) = delete;
		~Input()                    = delete;
		
		Input& operator = (const Input&  _other) = delete;
		Input& operator = (      Input&& _other) = delete;
		
		/** @brief Refresh the input. */
		static void Tick() {
			
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
			
			SDL_Event event { 0U };
			
			// Poll for pending events. Calling SDL_PollEvent also updates the internal keyboard state.
			while (SDL_PollEvent(&event) != 0) {
				
				// Send event to GUI for processing.
				UI::GUI::ProcessEvent(event);
				
				// Get list of events of the same type (if they exist).
				const auto existing = Input::Event::s_Events.Get(event.type);
				
				// Append the current event to the list.
				std::vector<SDL_Event> bucket;
				if (existing.has_value()) {
					bucket = existing.value();
					bucket.emplace_back(event);
				}
				else {
					bucket = { event };
				}
				
				// Update the list in the hashmap.
				Input::Event::s_Events.Assign(std::move(event.type), std::move(bucket));
			}
			
			/* HANDLE MOUSE STATE */
			{
				ivec2 delta;
				
				Input::Mouse::s_MouseState = SDL_GetRelativeMouseState(&delta.x, &delta.y);
				Input::Mouse::s_Motion = (static_cast<vec2>(delta) / Time::UnscaledDeltaTime<scalar_t>() / static_cast<scalar_t>(1000.0)) * static_cast<scalar_t>(0.1);
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
		
		/** @brief Finalise input. */
		static void Dispose() noexcept {
			
			try {
				Event::s_Events.Clear();
			}
			catch (const std::exception& e) {
				Debug::Log(e, LogType::Critical);
			}
			
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
		
	};
	
} // LouiEriksson::Engine::Input

#endif //FINALYEARPROJECT_INPUT_HPP