#include "stdafx.h"

#include "Cursor.h"

namespace LouiEriksson {
	
	void Cursor::State::Apply() const {
		
		// TODO: Currently a bug in SDL2 causes the cursor to be hidden while is
		// it set to relative. A official fix may be available in the near-future,
		// but a temporary solution may be to use this pull request:
		// https://github.com/libsdl-org/SDL/pull/7947/files
		SDL_ShowCursor(m_Visible ? SDL_ENABLE : SDL_DISABLE);
		
		// Determine whether or not the cursor should be relative to a window.
	    SDL_SetRelativeMouseMode(m_LockMode == LockMode::Absolute ? SDL_FALSE : SDL_TRUE);
		
		// If m_LockMode is LockMode::Centered, the cursor's position
		// must be re-centered to the window's midpoint each frame.
		if (m_LockMode == LockMode::Centered) {
			
			/*
			 * Check if the window pointer is valid.
			 * If it is valid, the mouse is centered to the provided window.
			 * If not, an error is thrown.
			 */
			if (auto window = m_Window.lock()) {
				
				// Get window dimensions.
				const auto dimensions = window->Dimensions();
				
				// Move cursor to window center.
				SDL_WarpMouseInWindow(window->operator SDL_Window *(), dimensions.x / 2, dimensions.y / 2);
			
				// Recalibrate the relative mouse position.
				SDL_GetRelativeMouseState(nullptr, nullptr);
			}
			else {
				throw std::runtime_error("m_Window is expired!");
			}
		}
	}
	
	Cursor::State::State(const std::weak_ptr<Window>& _window, const Cursor::State::LockMode& _lockMode, const bool& _visible) {
		
		m_Window   = _window;
		m_LockMode = _lockMode;
		m_Visible  = _visible;
	}
	
	Cursor::State::State() {
		
		m_Window   = {};
		m_LockMode = SDL_GetRelativeMouseMode() == SDL_ENABLE ? Cursor::State::LockMode::Relative : Cursor::State::LockMode::Absolute;
		m_Visible  = SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE;
	}
	
	Cursor::State Cursor::GetState() {
		return s_Current;
	}
	
	void Cursor::SetState(const Cursor::State& _state) {
		
		s_Current = _state;
		
		GetState().Apply();
	}
	
	void Cursor::Update() {
		GetState().Apply();
	}
	
	void Cursor::Reset() {
		SetState(s_Default);
	}
	
} // LouiEriksson