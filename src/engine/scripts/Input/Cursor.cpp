#include "../stdafx.h"

#include "Cursor.h"

namespace LouiEriksson {
	
	void Cursor::State::Apply() const {
		
		// Disable SDL influencing the mouse capture because when buttons are pressed.
		SDL_SetHint(SDL_HINT_MOUSE_AUTO_CAPTURE, m_LockMode == LockMode::Absolute ? "1" : "0");
		
		/*
		 * Make SDL's relative mode use warping, as the mouse seems
		 * to escape the window for a brief period of time otherwise.
		 */
		SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1");
		
		/*
		 * If m_LockMode is LockMode::Centered, the cursor's position
		 * must be re-centered to the window's midpoint each frame.
		 * we can achieve this using an SDL hint.
		 */
		SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, m_LockMode == LockMode::Centered ? "1" : "0");
		
		// Determine whether or not the cursor should be relative to a window.
	    SDL_SetRelativeMouseMode(m_LockMode == LockMode::Absolute ? SDL_FALSE : SDL_TRUE);
		
		// Apply the visibility of the cursor.
		SDL_ShowCursor(m_Visible ? SDL_ENABLE : SDL_DISABLE);
	}
	
	Cursor::State::State(const LockMode& _lockMode, const bool& _visible) {
		m_LockMode = _lockMode;
		m_Visible  = _visible;
	}
	
	Cursor::State::State() {
		m_LockMode = SDL_GetRelativeMouseMode() == SDL_ENABLE ? Cursor::State::LockMode::Relative : Cursor::State::LockMode::Absolute;
		m_Visible  = SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE;
	}
	
	void Cursor::SetState(const Cursor::State& _state) {
		s_Pending = _state;
	}
	
	Cursor::State Cursor::GetState() {
		return s_Current;
	}
	
	void Cursor::Update() {
		s_Pending.Apply();
		
		s_Current = s_Pending;
	}
	
	void Cursor::Reset() {
		SetState(s_Default);
	}
	
} // LouiEriksson