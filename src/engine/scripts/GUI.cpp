#include "stdafx.h"

#include "GUI.h"

namespace LouiEriksson {
	
	void GUI::Init(const std::weak_ptr<Window> _window, const char* _glsl_version) {
		
		// Initialise IMGUI:
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		
		ImGui_ImplSDL2_InitForOpenGL(_window.lock()->operator SDL_Window *(), _window.lock()->Context());
		ImGui_ImplOpenGL3_Init(_glsl_version);
	}
	
	void GUI::Style(const GUI::Parameters::Style& _style) {
		
		// Select a visual style:
		switch (_style) {
			
			case GUI::Parameters::Style::Light: { ImGui::StyleColorsLight(); break; }
			case GUI::Parameters::Style::Dark:  { ImGui::StyleColorsDark();  break; }
			default: {
				throw std::runtime_error("Not implemented!");
			}
		}
	}
	
	void GUI::OnGUI(const std::weak_ptr<Window> _window) {
		
		// Set up IMGUI to render a new frame.
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(_window.lock()->operator SDL_Window *());
		ImGui::NewFrame();
		
		// Draw a simple window:
		ImGui::Begin("Diagnostics");
			ImGui::Text("%s", ("FPS: " + std::to_string(1.0f / Time::DeltaTime())).c_str());
		ImGui::End();
		
		// Render output using OpenGL (3.x):
		ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	
	void GUI::Dispose() {
		
		// Safely dispose of the IMGUI context.
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		
		ImGui::DestroyContext();
	}
	
}