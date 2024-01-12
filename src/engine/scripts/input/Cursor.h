#ifndef FINALYEARPROJECT_CURSOR_H
#define FINALYEARPROJECT_CURSOR_H

namespace LouiEriksson {
	
	class Cursor {
		
		friend class Application;
		
	public:
		
		class State {
		
			friend class Cursor;
			
		public:
			
			/// <summary> Enum representing different ways the Cursor's movement can be restricted. </summary>
			enum LockMode : char {
				
				/// <summary>
				/// The Cursor is free to move without restriction.
				/// </summary>
				Absolute,
				
				/// <summary>
				/// The Cursor's position is confined to the window.
				/// </summary>
				Relative,
				
				/// <summary>
				/// The Cursor's position is locked to the center of the window.
				/// </summary>
				Centered
			};
			
			/// <summary> The lock mode of the Cursor. </summary>
			LockMode m_LockMode;
		
			/// <summary>
			/// Whether or not to hide the Cursor.
			/// <remarks>
			/// TODO: A bug in SDL2 causes the cursor to always be hidden while is it set
			/// to relative. An official fix may be available in the near-future, but a
			/// temporary solution may be to use this pull request:
			/// https://github.com/libsdl-org/SDL/pull/7947/files
			/// </remarks>
			/// </summary>
			bool m_Visible;
		
			State(const LockMode& _lockMode, const bool& _visible);
			
		private:
			
			State();
			
			void Apply() const;
		};
		
		/// <summary>
		/// Set the pending state of the Cursor.
		/// <remarks>
		/// Please note: Your changes will not take effect until the beginning of the
		/// next frame, and may be overridden by subsequent calls to this function.
		/// </remarks>
		/// </summary>
		static void SetState(const State& _state);
		
		/// <summary>
		/// Return the current state of the Cursor.
		/// <remarks>
		/// Please note: The result of GetState() may not always match the value
		/// provided to SetState(). This is because the Cursor's state does not
		/// get applied until the beginning of the next frame. When GetState()
		/// is called, it returns the current state of the Cursor, regardless
		/// of whatever state changes may be pending.
		/// </remarks>
		/// </summary>
		static State GetState();
		
	private:
		
		inline const static Cursor::State s_Default{}; // Default Cursor state.
		inline       static Cursor::State s_Pending{}; // Pending Cursor state. Will be applied next frame.
		inline       static Cursor::State s_Current{}; // The actual current state of the Cursor.
		
		/// <summary> Update the state of the Cursor, applying any pending changes. </summary>
		static void Update();
		
		/// <summary> Reset the Cursor to its default state. </summary>
		static void Reset();
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_CURSOR_H