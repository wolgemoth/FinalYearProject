#ifndef FINALYEARPROJECT_INPUT_H
#define FINALYEARPROJECT_INPUT_H

#include "stdafx.h"

class Application;

// @Assessor: This class was originally submitted for 3DGP and modified for GEP.

namespace LouiEriksson {
	
	class Input {
	
	friend class Application;
	
	private:
		
		inline static Hashmap<Uint32, std::vector<SDL_Event>> s_Events;

		inline static const Uint8* s_KeyboardState = nullptr;
		
	public:
	
		static bool Get(const Uint32& _event, std::vector<SDL_Event>& _results);
		
		static bool Get(const Uint32& _event);
		
		static bool GetKey(const SDL_Scancode& _key);
		
		 Input()                    = delete;
		 Input(const Input& _other) = delete;
		~Input()                    = delete;
		
		Input& operator = (const Input&  _other) = delete;
		Input& operator = (      Input&& _other) = delete;
		
		static void Tick();
		
	};
}

#endif //FINALYEARPROJECT_INPUT_H