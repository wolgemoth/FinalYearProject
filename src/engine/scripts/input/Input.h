#ifndef FINALYEARPROJECT_INPUT_H
#define FINALYEARPROJECT_INPUT_H

#include "../utils/Hashmap.h"

#include <glm/ext/vector_float2.hpp>

#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>

#include <vector>

class Application;

// @Assessor: This class was originally submitted for 3DGP and modified for GEP.

namespace LouiEriksson {
	
	class Input {
	
	friend class Application;
	
	private:
	
	public:
	
		struct Event {
		
			friend Input;
			friend class Mouse;
		
		private:
		
			inline static Hashmap<Uint32, std::vector<SDL_Event>> s_Events;

		public:
		
			static bool Get(const Uint32& _event, std::vector<SDL_Event>& _results);
			
			static bool Get(const Uint32& _event);
			
		};
		
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
				
				 State();
				~State();
				
				/// <summary> Returns the state of the keyboard at the provided SDL_KeyCode. </summary>
				[[nodiscard]]bool Get(const SDL_Keycode& _value) const;
				
				/// <summary> Returns the state of the keyboard at the provided SDL_Scancode. </summary>
				[[nodiscard]]bool Get(const SDL_Scancode& _value) const;
			};
			
			inline static State s_PreviousKeyboardState{};
			inline static State  s_CurrentKeyboardState{};
		
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
		
		struct Mouse {
		
			friend Input;
		
		private:
			
			inline static Uint32 s_RelativeState { 0u };
			
			inline static glm::vec2 s_Motion { 0.0f, 0.0f };
			
		public:
			
			/// <summary> Returns the relative motion of the mouse. </summary>
			static const glm::vec2& Motion();
			
			/// <summary> Returns true if the provided mouse button (i.e SDL_BUTTON_LEFT) is held. </summary>
			static const bool Get(const Uint8& _button);
			
			/// <summary> Returns true if the provided mouse button (i.e SDL_BUTTON_LEFT) was pressed. </summary>
			static const bool GetDown(const Uint8& _button);
			
			/// <summary> Returns true if the provided mouse button (i.e SDL_BUTTON_LEFT) was released. </summary>
			static const bool GetUp(const Uint8& _button);
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
}

#endif //FINALYEARPROJECT_INPUT_H