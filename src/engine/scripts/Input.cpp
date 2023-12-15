#include "stdafx.h"

#include "Input.h"

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	void Input::KeyboardState(const Uint8* _state) {
		s_KeyboardState = _state;
	}
	
	const Uint8* Input::KeyboardState() {
		return s_KeyboardState;
	}
}