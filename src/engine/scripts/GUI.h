#ifndef FINALYEARPROJECT_GUI_H
#define FINALYEARPROJECT_GUI_H

#include "stdafx.h"

#include "Application.h"

namespace LouiEriksson {
	
	class GUI {
	
		friend Application;
		
	private:
	
		struct Parameters {
			
			enum Style {
				Dark,
				Light
			};
		};
		
		class GUIWindows {
			
			friend GUI;
			
		private:
			
			// Constant for the margins between the window and screen edge.
			inline static const ImVec2 s_WindowMargin = ImVec2(16, 16);
			
			static void DrawDiagnosticsWindow(const std::weak_ptr<Window>& _window);
			
			static void DrawPostProcessingWindow(const std::weak_ptr<Window>& _window);
			
			static void DrawRenderSettingsWindow(const std::weak_ptr<Window>& _window);
			
		};
		
		static void Init(const std::weak_ptr<Window>& _window, const char* _glsl_version);
		
		static void Style(const GUI::Parameters::Style& _style);
		
		static void ProcessEvent(const SDL_Event& _event);
		
		static void OnGUI(const std::weak_ptr<Window>& _window);
		
		static void Dispose();
	};
}

#endif //FINALYEARPROJECT_GUI_H