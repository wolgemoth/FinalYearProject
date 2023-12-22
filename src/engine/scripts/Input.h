#ifndef FINALYEARPROJECT_INPUT_H
#define FINALYEARPROJECT_INPUT_H

#include "stdafx.h"

class Application;

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	class Input {
	
	friend class Application;
	
	private:
		
		inline static Hashmap<Uint32, std::vector<SDL_Event>> s_Events;

		inline static const Uint8* s_KeyboardState = nullptr;
		
	public:
	
		static bool Get(const Uint32& _event, std::vector<SDL_Event>& _results);
		
		static bool Get(const Uint32& _event);
		
		static const Uint8* KeyboardState();
		
		 Input()                    = delete;
		 Input(const Input& _other) = delete;
		~Input()                    = delete;
		
		Input& operator = (const Input&  _other) = delete;
		Input& operator = (      Input&& _other) = delete;
		
		static void Tick();
		
	};
}

#endif //FINALYEARPROJECT_INPUT_H