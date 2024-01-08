#ifndef FINALYEARPROJECT_INPUT_H
#define FINALYEARPROJECT_INPUT_H

#include "stdafx.h"

class Application;

// @Assessor: This class was originally submitted for 3DGP and modified for GEP.

namespace LouiEriksson {
	
	class Input {
	
	friend class Application;
	
	private:
	
	public:
	
		struct Event {
		
			friend Input;
		
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
			
			inline static State s_PreviousKeyboardState = State();
			inline static State  s_CurrentKeyboardState = State();
		
		public:
			
			static bool Get(const SDL_Scancode& _key);
			
			static bool Get(const SDL_Keycode& _key);
			
			static bool GetDown(const SDL_Scancode& _key);
			
			static bool GetDown(const SDL_Keycode& _key);
			
			static bool GetUp(const SDL_Scancode& _key);
			
			static bool GetUp(const SDL_Keycode& _key);
		};
		
		struct Mouse {
		
			friend Input;
		
		private:
			
			inline static glm::vec2 s_Motion = { 0, 0 };
			
		public:
			
			static const glm::vec2& Motion();
			
		};
		
		 Input()                    = delete;
		 Input(const Input& _other) = delete;
		~Input()                    = delete;
		
		Input& operator = (const Input&  _other) = delete;
		Input& operator = (      Input&& _other) = delete;
		
		static void Tick();
		
		static void Dispose();
	};
}

#endif //FINALYEARPROJECT_INPUT_H