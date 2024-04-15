#ifndef FINALYEARPROJECT_CURSOR_H
#define FINALYEARPROJECT_CURSOR_H

namespace LouiEriksson::Engine {
	
	class Application;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Input {
	
	class Cursor {
		
		friend LouiEriksson::Engine::Application;
		
	public:
		
		class State {
		
			friend Cursor;
			
		public:
			
			/**
			 * @brief Enumeration representing different ways the Cursor's movement can be restricted.
			 */
			enum LockMode : char {
				Absolute, /**< @brief The Cursor is free to move without restriction. */
				Relative, /**< @brief The Cursor's position is confined to the window. */
				Centered  /**< @brief The Cursor's position is locked to the center of the window. */
			};
			
			/** @brief The lock mode of the Cursor. */
			LockMode m_LockMode;
		
			/** @brief Whether or not to hide the Cursor. */
			bool m_Visible;
		
			State(const LockMode& _lockMode, const bool& _visible);
			
		private:
			
			State();
			
			void Apply() const;
			
		};
		
		/**
		 * @brief Set the pending state of the Cursor.
		 * @param[in] _state The new state of the Cursor.
		 *
		 * @note Your changes will not take effect until the beginning of the next frame, and may be overridden by subsequent calls to this function.
		 */
		static void SetState(const State& _state);
		
		/**
		 * @brief Returns the current state of the Cursor.
		 *
		 * @note The result of GetState() may not always match the value provided to SetState().
		 * This is because the Cursor's state does not get applied until the beginning of the next frame.
		 * When GetState() is called, it returns the current state of the Cursor, regardless of whatever state
		 * changes may be pending.
		 *
		 * @return The current state of the Cursor.
		 */
		static const State& GetState();
		
	private:
		
		inline const static Cursor::State s_Default{}; /**< @brief Default Cursor state. */
		inline       static Cursor::State s_Pending{}; /**< @brief Pending Cursor state. Will be applied next frame. */
		inline       static Cursor::State s_Current{}; /**< @brief The actual current state of the Cursor. */
		
		/** @brief Update the state of the Cursor, applying any pending changes. */
		static void Update();
		
		/** @brief Reset the Cursor to its default state. */
		static void Reset();
		
	};
	
} // LouiEriksson::Engine::Input

#endif //FINALYEARPROJECT_CURSOR_H