#ifndef FINALYEARPROJECT_INPUT_H
#define FINALYEARPROJECT_INPUT_H

#include "../core/utils/Hashmap.h"

#include <glm/ext/vector_float2.hpp>

#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>

#include <vector>

namespace LouiEriksson::Engine {
	
	class Application;
	class Window;
	
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
		
			/** @brief et all events of a certain type which occurred this frame. */
			static bool Get(const Uint32& _event, std::vector<SDL_Event>& _results) noexcept;
			
			/** @brief Check if an event occurred this frame. */
			static bool Get(const Uint32& _event) noexcept;
			
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
				
				 State() noexcept;
				~State();
				
				/** @brief  Returns the state of the keyboard at the provided SDL_KeyCode. */
				[[nodiscard]] bool Get(const SDL_Keycode& _value) const;
				
				/** @brief Returns the state of the keyboard at the provided SDL_Scancode. */
				[[nodiscard]] bool Get(const SDL_Scancode& _value) const;
				
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
			static bool Get(const SDL_Scancode& _key);
			
			/**
			 * @brief Returns true if the provided key (in the form of SDL_Keycode) is held.
			 *
			 * This function checks if the provided key is currently being held.
			 *
			 * @param[in] _key The SDL_Keycode of the key to check.
			 * @return True if the key is pressed, false otherwise.
			 */
			 static bool Get(const SDL_Keycode& _key);
			
			/**
			 * @brief Returns true if the provided key (in the form of SDL_Scancode) was pressed.
			 *
			 * This function checks if the provided key was released.
			 *
			 * @param[in] _key The SDL_Scancode of the key to check.
			 * @return True if the key is pressed, false otherwise.
			 */
			static bool GetDown(const SDL_Scancode& _key);
			
			/**
			 * @brief Returns true if the provided key (in the form of SDL_Keycode) was pressed.
			 *
			 * This function checks if the provided key was released.
			 *
			 * @param[in] _key The SDL_Keycode of the key to check.
			 * @return True if the key is pressed, false otherwise.
			 */
			static bool GetDown(const SDL_Keycode& _key);
			
			/**
			 * @brief Returns true if the provided key (in the form of SDL_Scancode) was released.
			 *
			 * This function checks if the provided key was previously pressed and is now released.
			 *
			 * @param[in] _key The SDL_Scancode of the key to check.
			 * @return True if the key is released, false otherwise.
			 */
			static bool GetUp(const SDL_Scancode& _key);
			
			/**
			 * @brief Returns true if the provided key (in the form of SDL_Keycode) was released.
			 *
			 * This function checks if the provided key was previously pressed and is now released.
			 *
			 * @param[in] _key The SDL_Keycode of the key to check.
			 * @return True if the key is released, false otherwise.
			 */
			static bool GetUp(const SDL_Keycode& _key);
			
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
			inline static Uint32 s_MouseState { 0u };
			
			/** @brief Pixels the mouse has moved since previous tick. */
			inline static glm::vec2 s_Motion { 0.0, 0.0 };
			
		public:
			
			/**
			 * @fn const glm::vec2& Input::Mouse::Motion() noexcept
			 * @brief Retrieves the motion of the mouse.
			 * @return The pixels the mouse has moved since the previous frame.
			 */
			static const glm::vec2& Motion() noexcept;
			
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
			static bool Get(const Uint8& _button) noexcept;
			
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
			static bool GetDown(const Uint8& _button);
			
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
			static bool GetUp(const Uint8& _button);
			
		};
		
		 Input()                    = delete;
		 Input(const Input& _other) = delete;
		~Input()                    = delete;
		
		Input& operator = (const Input&  _other) = delete;
		Input& operator = (      Input&& _other) = delete;
		
		/** @brief Refresh the input. */
		static void Tick();
		
		/** @brief Finalise input. */
		static void Dispose();
		
	};
	
} // LouiEriksson::Engine::Input

#endif //FINALYEARPROJECT_INPUT_H