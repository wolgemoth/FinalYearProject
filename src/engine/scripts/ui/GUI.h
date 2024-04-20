#ifndef FINALYEARPROJECT_GUI_H
#define FINALYEARPROJECT_GUI_H

#include <imgui.h>
#include <SDL_events.h>

#include <memory>

namespace LouiEriksson::Engine {
	
	class Application;
	class Window;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Input {
	
	class Input;
	
} // LouiEriksson::Engine::Input

namespace LouiEriksson::Engine::UI {
	
	class GUI final {
		
		friend LouiEriksson::Engine::Application;
		friend LouiEriksson::Engine::Input::Input;
		
	private:
	
		struct Parameters final {
			
			enum Style : unsigned char {
				Dark  = 0u, /**< @brief  Dark theme. */
				Light = 1u, /**< @brief Light theme. */
			};
			
		};
		
		class GUIWindows final {
			
			friend GUI;
			
		private:
			
			/** @brief Constant for the margins between the window and screen edge. */
			inline static const ImVec2 s_WindowMargin { 16, 16 };
			
			static void DiagnosticsWindow(const std::weak_ptr<Window>& _window, const bool& _draw = true);
			
			static void PostProcessingWindow(const std::weak_ptr<Window>& _window, const bool& _draw = true);
			
			static void RenderSettingsWindow(const std::weak_ptr<Window>& _window, const bool& _draw = true);
		
		};
		
		inline static bool s_DrawDebugWindows { false };
		
		/**
		 * @brief Initialises the GUI.
		 *
		 * This function is responsible for initialising the GUI using the ImGui library.
		 * It creates the ImGui context, initialises the ImGui backends for the specified window, and sets up the OpenGL bindings.
		 *
		 * @param[in] _window The weak pointer to the Window object.
		 * @param[in] _glsl_version The GLSL version to use.
		 */
		static void Init(const std::weak_ptr<Window>& _window, const char* _glsl_version);
		
		/**
		 * @brief Set the GUI to a specific color palette.
		 *
		 * This function sets the GUI to a specific color palette based on the given style parameter.
		 *
		 * @param[in] _style The style parameter to set the GUI color palette.
		 */
		static void Style(const GUI::Parameters::Style& _style);
		
		/**
		 * @brief Processes a given SDL_Event.
		 *
		 * @param[in] _event The SDL_Event to process.
		 */
		static void ProcessEvent(const SDL_Event& _event);
		
		/**
		* @brief Main GUI loop.
		* @param[in] _window The weak pointer to the Window object.
		*/
		static void OnGUI(const std::weak_ptr<Window>& _window);
		
		/**
		 * @brief Dispose of the GUI.
		 *
		 * This function is responsible for disposing the GUI, freeing up any resources that were allocated during initialisation.
		 * It safely disposes of the IMGUI context, shuts down the ImGui backends, and destroys the context.
		 */
		static void Dispose();
		
	};
	
} // LouiEriksson::Engine::UI

#endif //FINALYEARPROJECT_GUI_H