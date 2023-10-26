#pragma once

#ifndef FINALYEARPROJECT_INPUT_H
#define FINALYEARPROJECT_INPUT_H

#include "stdafx.h"

class Application;

namespace LouiEriksson {
	
	class Input {
	
	friend class Application;
	
	private:
		
		inline static const Uint8* s_KeyboardState = nullptr;
	
		static void KeyboardState(const Uint8* _state);
	
	public:
		
		static const Uint8* KeyboardState();
		
		 Input()                    = delete;
		 Input(const Input& _other) = delete;
		~Input()                    = delete;
		
		Input& operator = (const Input&  _other) = delete;
		Input& operator = (      Input&& _other) = delete;
		
	};
}

#endif //FINALYEARPROJECT_INPUT_H