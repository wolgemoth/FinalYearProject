#ifndef FINALYEARPROJECT_CURSOR_H
#define FINALYEARPROJECT_CURSOR_H

namespace LouiEriksson::Engine {
	
	class Application;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Input {
	
	class Cursor final {
		
		friend LouiEriksson::Engine::Application;
		
	public:
		
		class State final {
		
			friend Cursor;
			
		public:
			
			/**
			 * @brief Enumeration representing different ways the Cursor's movement can be restricted.
			 */
			enum LockMode : unsigned char {
				Absolute, /**< @brief The Cursor is free to move without restriction. */
				Relative, /**< @brief The Cursor's position is confined to the window. */
				Centered  /**< @brief The Cursor's position is locked to the center of the window. */
			};
			
			/** @brief The lock mode of the Cursor. */
			LockMode m_LockMode;
		
			/** @brief Whether or not to hide the Cursor. */
			bool m_Visible;
		
			constexpr State(const LockMode& _lockMode, const bool& _visible) noexcept :
				m_LockMode(_lockMode),
				m_Visible (_visible) {}
			
		private:
			
			State() :
				m_LockMode(SDL_GetRelativeMouseMode() == SDL_ENABLE ? Cursor::State::LockMode::Relative : Cursor::State::LockMode::Absolute),
				m_Visible (SDL_ShowCursor(SDL_QUERY)  == SDL_ENABLE) {}
		
			void Apply() const {
		
				// Disable SDL influencing the mouse capture because when buttons are pressed.
				SDL_SetHint("SDL_HINT_MOUSE_AUTO_CAPTURE", m_LockMode == LockMode::Absolute ? "1" : "0");
				
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
				SDL_SetHint("SDL_HINT_MOUSE_RELATIVE_MODE_CENTER", m_LockMode == LockMode::Centered ? "1" : "0");
				
				// Determine whether or not the cursor should be relative to a window.
			    SDL_SetRelativeMouseMode(m_LockMode == LockMode::Absolute ? SDL_FALSE : SDL_TRUE);
				
				// Apply the visibility of the cursor.
				SDL_ShowCursor(m_Visible ? SDL_ENABLE : SDL_DISABLE);
			}
			
		};
		
		/**
		 * @brief Set the pending state of the Cursor.
		 * @param[in] _state The new state of the Cursor.
		 *
		 * @note Your changes will not take effect until the beginning of the next frame, and may be overridden by subsequent calls to this function.
		 */
		static void SetState(const State& _state) {
			s_Pending = _state;
		}
		
		/**
		 * @brief Returns the current state of the Cursor.
		 *
		 * @note The result of GetState() may not always match the value provided to SetState().
		 * This is because the Cursor's state does not value applied until the beginning of the next frame.
		 * When GetState() is called, it returns the current state of the Cursor, regardless of whatever state
		 * changes may be pending.
		 *
		 * @return The current state of the Cursor.
		 */
		static constexpr const State& GetState() {
			return s_Current;
		}
		
	private:
		
		inline static const Cursor::State s_Default{}; /**< @brief Default Cursor state. */
		inline static       Cursor::State s_Pending{}; /**< @brief Pending Cursor state. Will be applied next frame. */
		inline static       Cursor::State s_Current{}; /**< @brief The actual current state of the Cursor. */
		
		/** @brief Update the state of the Cursor, applying any pending changes. */
		static void Update() {
			s_Pending.Apply();
			
			s_Current = s_Pending;
		}
		
		/** @brief Reset the Cursor to its default state. */
		static void Reset() {
			SetState(s_Default);
		}
		
	};
	
} // LouiEriksson::Engine::Input

#endif //FINALYEARPROJECT_CURSOR_H