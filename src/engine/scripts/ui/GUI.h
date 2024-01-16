#ifndef FINALYEARPROJECT_GUI_H
#define FINALYEARPROJECT_GUI_H

#include <imgui.h>
#include <SDL_events.h>

#include <memory>

// @Assessor: This class was submitted for GACP. Please don't mark it for GEP.

namespace LouiEriksson::Engine {
	
	class Application;
	class Window;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Input {
	
	class Input;
	
} // LouiEriksson::Engine::Input

namespace LouiEriksson::Engine::UI {
	
	class GUI {
		
		friend LouiEriksson::Engine::Application;
		friend LouiEriksson::Engine::Input::Input;
		
	private:
	
		struct Parameters {
			
			enum Style : char {
				
				/// <summary> Dark theme. </summary>
				Dark,
				
				/// <summary> Light theme. </summary>
				Light
			};
			
		};
		
		class GUIWindows {
			
			friend GUI;
			
		private:
			
			// Constant for the margins between the window and screen edge.
			inline static const ImVec2 s_WindowMargin { 16, 16 };
			
			static void DiagnosticsWindow(const std::weak_ptr<Window>& _window, const bool& _draw = true);
			
			static void PostProcessingWindow(const std::weak_ptr<Window>& _window, const bool& _draw = true);
			
			static void RenderSettingsWindow(const std::weak_ptr<Window>& _window, const bool& _draw = true);
		
		};
		
		inline static bool s_DrawDebugWindows { false };
		
		/// <summary> Initialise the GUI. </summary>
		static void Init(const std::weak_ptr<Window>& _window, const char* _glsl_version);
		
		/// <summary> Set the GUI to a specific color palette. </summary>
		static void Style(const GUI::Parameters::Style& _style);
		
		/// <summary> Processes a given SDL_Event. </summary>
		static void ProcessEvent(const SDL_Event& _event);
		
		/// <summary> Main loop. </summary>
		static void OnGUI(const std::weak_ptr<Window>& _window);
		
		/// <summary> Reset of the GUI. </summary>
		static void Dispose();
		
	};
	
} // LouiEriksson::Engine::UI

#endif //FINALYEARPROJECT_GUI_H