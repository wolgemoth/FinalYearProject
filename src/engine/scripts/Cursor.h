#ifndef FINALYEARPROJECT_CURSOR_H
#define FINALYEARPROJECT_CURSOR_H

#include "Window.h"

namespace LouiEriksson {
	
	class Cursor {
	
		friend class Application;
		
	public:
		
		class State {
		
			friend class Cursor;
			
		public:
			
			enum LockMode {
				Absolute,
				Relative,
				Centered
			};
			
			std::weak_ptr<Window> m_Window;
			
			LockMode m_LockMode;
		
			bool m_Visible;
		
			State(const std::weak_ptr<Window>& _window, const LockMode& _lockMode, const bool _visible);
			
		private:
			
			State();
			
			void Apply() const;
		};
		
		static State GetState();
		
		static void SetState(const State& _state);
		
	private:
		
		inline const static Cursor::State s_Default = State();
		inline       static Cursor::State s_Current = State();
		
		static void Update();
		
		static void Reset();
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_CURSOR_H