#include "stdafx.h"

#include "Input.h"

namespace LouiEriksson {
	
	void Input::KeyboardState(const Uint8* _state) {
		s_KeyboardState = _state;
	}
	
	const Uint8* Input::KeyboardState() {
		return s_KeyboardState;
	}
}