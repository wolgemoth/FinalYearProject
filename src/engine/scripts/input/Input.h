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
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Input {
	
	class Input {
	
		friend LouiEriksson::Engine::Application;
	
	private:
	
	public:
	
		/// <summary> Provides an interface for detecting SDL_Events. </summary>
		struct Event {
		
			friend Input;
		
		private:
		
			/// <summary> Hashmap of all SDL_Events for this frame. </summary>
			inline static Hashmap<Uint32, std::vector<SDL_Event>> s_Events;

		public:
		
			/// <summary> Get all events of a certain type which occurred this frame. </summary>
			static bool Get(const Uint32& _event, std::vector<SDL_Event>& _results) noexcept;
			
			/// <summary> Check if an event occurred this frame. </summary>
			static bool Get(const Uint32& _event) noexcept;
			
		};
	
		/// <summary> Provides an interface for detecting key presses. </summary>
		struct Key {
		
			friend Input;
		
		private:
			
			/// <summary> Wraps a c-style pointer representing the state of the keyboard. </summary>
			struct State {
			
				friend Input;
				
			private:
				
				const Uint8* s_Data;
				int s_Length;
				
			public:
				
				 State() noexcept;
				~State();
				
				/// <summary> Returns the state of the keyboard at the provided SDL_KeyCode. </summary>
				[[nodiscard]] bool Get(const SDL_Keycode& _value) const;
				
				/// <summary> Returns the state of the keyboard at the provided SDL_Scancode. </summary>
				[[nodiscard]] bool Get(const SDL_Scancode& _value) const;
				
			};
			
			inline static State s_PreviousKeyboardState{}; // Keyboard state previous tick.
			inline static State  s_CurrentKeyboardState{}; // Keyboard state current tick.
		
		public:
			
			/// <summary> Returns true if the provided key (in the form of SDL_Scancode) is held. </summary>
			static bool Get(const SDL_Scancode& _key);
			
			/// <summary> Returns true if the provided key (in the form of SDL_Keycode) is held. </summary>
			static bool Get(const SDL_Keycode& _key);
			
			/// <summary> Returns true if the provided key (in the form of SDL_Scancode) was pressed. </summary>
			static bool GetDown(const SDL_Scancode& _key);
			
			/// <summary> Returns true if the provided key (in the form of SDL_Keycode) was pressed. </summary>
			static bool GetDown(const SDL_Keycode& _key);
			
			/// <summary> Returns true if the provided key (in the form of SDL_Scancode) was released. </summary>
			static bool GetUp(const SDL_Scancode& _key);
			
			/// <summary> Returns true if the provided key (in the form of SDL_Keycode) was released. </summary>
			static bool GetUp(const SDL_Keycode& _key);
			
		};
		
		/// <summary> Provides an interface for detecting mouse input. </summary>
		struct Mouse {
		
			friend Input;
		
		private:
			
			/// <summary> SDL mouse relative state. </summary>
			inline static Uint32 s_RelativeState { 0u };
			
			/// <summary> Reinitialise the mouse has moved since previous tick. </summary>
			inline static glm::vec2 s_Motion { 0.0f, 0.0f };
			
		public:
			
			/// <summary> Returns the relative motion of the mouse. </summary>
			static const glm::vec2& Motion() noexcept;
			
			/// <summary> Returns true if the provided mouse button (i.e SDL_BUTTON_LEFT) is held. </summary>
			static bool Get(const Uint8& _button) noexcept;
			
			/// <summary> Returns true if the provided mouse button (i.e SDL_BUTTON_LEFT) was pressed. </summary>
			static bool GetDown(const Uint8& _button);
			
			/// <summary> Returns true if the provided mouse button (i.e SDL_BUTTON_LEFT) was released. </summary>
			static bool GetUp(const Uint8& _button);
			
		};
		
		 Input()                    = delete;
		 Input(const Input& _other) = delete;
		~Input()                    = delete;
		
		Input& operator = (const Input&  _other) = delete;
		Input& operator = (      Input&& _other) = delete;
		
		/// <summary> Refresh the input. </summary>
		static void Tick();
		
		/// <summary> Finalise input. </summary>
		static void Dispose();
		
	};
	
} // LouiEriksson::Engine::Input

#endif //FINALYEARPROJECT_INPUT_H