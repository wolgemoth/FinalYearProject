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
	
	void GUI::ProcessEvent(const SDL_Event& _event) {
		ImGui_ImplSDL2_ProcessEvent(&_event);
	}
	
	void GUI::OnGUI(const std::weak_ptr<Window> _window) {
		
		/* INIT GUI FRAME */
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(_window.lock()->operator SDL_Window *());
		ImGui::NewFrame();
		
		// Diagnostics window:
		{
			// Set default values (on first run):
			ImGui::SetNextWindowSize(ImVec2(256, 256), ImGuiCond_Once);
			ImGui::SetNextWindowPos(ImVec2(16, 16), ImGuiCond_Once);
			ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
			
			// Draw elements:
			ImGui::Begin("Diagnostics", nullptr);
				ImGui::Text("FPS: %.1f", 1.0f / Time::DeltaTime());
			ImGui::End();
		}
		
		/* FINALIZE GUI FRAME */
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