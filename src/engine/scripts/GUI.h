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
		
		inline static float s_Plot_SamplingWindowSize = 20.0f;
		inline static float s_FPS_SamplingWindowSize = 0.5f;
		
		static void Init(std::weak_ptr<Window> _window, const char* _glsl_version);
		
		static void Style(const GUI::Parameters::Style& _style);
		
		static void ProcessEvent(const SDL_Event& _event);
		
		static void OnGUI(std::weak_ptr<Window> _window);
		
		static void DrawDiagnosticsWindow(std::weak_ptr<Window> _window);
		
		static void DrawParametersWindow(std::weak_ptr<Window> _window);
		
		static void Dispose();
	};
}

#endif //FINALYEARPROJECT_GUI_H