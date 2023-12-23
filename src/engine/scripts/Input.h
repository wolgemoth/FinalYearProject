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
			
			inline static const Uint8* s_KeyboardState = nullptr;
		
		public:
			
			static bool Get(const SDL_Scancode& _key);
			
			static bool Get(const SDL_Keycode& _key);
			
		};
		
		struct Mouse {
		
			friend Input;
		
		private:
			
			inline static glm::vec2 s_Motion = { 0, 0 };
			
		public:
			
			static glm::vec2 Motion();
			
		};
		
		 Input()                    = delete;
		 Input(const Input& _other) = delete;
		~Input()                    = delete;
		
		Input& operator = (const Input&  _other) = delete;
		Input& operator = (      Input&& _other) = delete;
		
		static void Tick();
		
	};
}

#endif //FINALYEARPROJECT_INPUT_H