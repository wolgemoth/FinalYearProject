#ifndef FINALYEARPROJECT_INPUT_H
#define FINALYEARPROJECT_INPUT_H

#include "stdafx.h"

#include "GUI.h"

class Application;

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	class Input {
	
	friend class Application;
	
	private:
		
		struct SDL_Event_Hash {
			
			std::size_t operator() (const SDL_Event& _item) const;
		};
		
		struct SDL_Event_Comparator : std::equal_to<SDL_Event> {
			
			bool operator() (const SDL_Event& _a, const SDL_Event& _b) const;
		};
		
		static std::unordered_set<SDL_Event, SDL_Event_Hash, SDL_Event_Comparator> s_Events;
		
		inline static const Uint8* s_KeyboardState = nullptr;
		
	public:
	
		static bool HasEvent(const SDL_Event& _event);
		
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