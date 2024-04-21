#ifndef FINALYEARPROJECT_APPLICATION_H
#define FINALYEARPROJECT_APPLICATION_H

#include "../audio/Sound.h"
#include "../core/Time.h"
#include "../core/utils/Utils.h"
#include "../ecs/Scene.h"
#include "../input/Cursor.h"
#include "../input/Input.h"
#include "../networking/Requests.h"
#include "../physics/Physics.h"
#include "../ui/GUI.h"
#include "Types.h"
#include "utils/Hashmap.h"

#include "Debug.h"
#include "Resources.h"
#include "Settings.h"
#include "Window.h"

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_mouse.h>
#include <SDL_stdinc.h>
#include <SDL_video.h>

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

namespace LouiEriksson::Engine::ECS {
	
	class GameObject;
	
}; // LouiEriksson::Engine::ECS

namespace LouiEriksson::Engine {

	class Script;
	
	/**
	 * @class Application
	 * @brief Represents the application.
	 *
	 * The Application class is responsible for managing the main execution flow of the program and handling the termination of the application.
	 */
	class Application final {
	
	private:
	
		inline static std::atomic<bool> s_Quit        { false }; // Is Application scheduled to quit?
		inline static std::atomic<bool> s_Initialised { false }; // Is Application initialised?
		
		/**
		 * @brief Finalises the application.
		 *
		 * Releases allocated resources.
		 *
		 * @see Debug::Log(std::string_view, LogType, bool)
		 * @see Input::Cursor::Reset()
		 * @see UI::GUI::Dispose()
		 * @see Physics::Physics::Dispose()
		 * @see Input::Input::Dispose()
		 * @see Audio::Sound::Dispose()
		 * @see Networking::Requests::Dispose()
		 * @see SDL_Quit()
		 * @see Debug::Flush()
		 *
		 * @note This function should only be called when the application is about to terminate.
		 */
		static void Finalise()  {
		
			Debug::Log("Application::Finalise()", LogType::Info);
			
			Input::Cursor::Reset();
			
			        UI::     GUI::Dispose();
			   Physics:: Physics::Dispose();
			     Input::   Input::Dispose();
			     Audio::   Sound::Dispose();
			Networking::Requests::Dispose();
			
		    try {
				SDL_Quit();
				
				Utils::SDLDumpError();
			}
			catch (const std::exception& e) {
				Debug::Log(e, LogType::Critical);
			}
			
			Debug::Flush();
		}
		
		/**
		 * @brief Called when the application unexpectedly terminates.
		 *
		 * Perform finalisation and then exit the application.
		 *
		 * @see Debug::Log(const std::string_view&, const LogType&, const bool&)
		 * @see Utils::ALDumpError(const bool&)
		 * @see Utils::GLDumpError(const bool&)
		 * @see Application::Finalise()
		 * @see std::exit()
		 */
		static void OnTerminate()  {
		
			Debug::Log("Application::OnTerminate()", LogType::Critical);
			
			try {
				Utils::ALDumpError();
				Utils::GLDumpError();
				Utils::SDLDumpError();
				
				Finalise();
			}
			catch (const std::exception& e) {
				Debug::Log(e, LogType::Critical);
			}
			
			std::exit(1);
		}
		
	public:
	
		 Application()                          = delete;
		 Application(const Application& _other) = delete;
		~Application()                          = delete;
		
		Application& operator = (const Application&  _other) = delete;
		Application& operator =       (Application&& _other) = delete;
		
		/**
		 * @brief Entry point of the application.
		 *
		 * This function is the main entry point of the application, and contains the main loop.
		 *
		 * @param[in] _values A hashmap containing function pointers to initialise different user scripts
		 * @return An integer error code (0 for successful execution)
		 */
		static int Main(const Hashmap<std::string, std::shared_ptr<Script> (*)(const std::weak_ptr<ECS::GameObject>& _parent)>& _initialisers) {
			
			// Restrict Main() to one instance.
			if (s_Initialised) {
				Debug::Log("Attempted to call Application::Main() while it is already running! Do you have multiple instances?", LogType::Warning);
			}
			else {
				s_Quit        = false;
				s_Initialised =  true;
				
				Debug::Log("Application Initialising.", LogType::Info);
			
				// Set custom termination behaviour:
				std::set_terminate(OnTerminate);
				
				try {
					
					srand(0u); // Use a constant seed (like '0') for deterministic behaviour.
					
					/* INIT */
					auto main_window = Window::Create(1280, 720, "FinalYearProject");
					
					{
						const auto glew_status = glewInit();
						
						if (glew_status != GLEW_OK) {
							
							// Throw error if GLEW fails to initialise correctly.
							throw std::runtime_error("Failed to initialise GLEW! Status: [" + std::to_string(glew_status) + "]");
						}
					}
					
					auto renderFlags = Graphics::Camera::RenderFlags::REINITIALISE;
					
					Input::Cursor::SetState({ Input::Cursor::State::LockMode::Absolute, true });
					
					// Capture the mouse on startup.
					SDL_CaptureMouse(SDL_TRUE);
					
					     Audio::    Sound::Init();
					            Resources::Init();
					             Settings::Init();
					   Physics::  Physics::Init();
					Networking:: Requests::Init();
					
					UI::GUI::Init(main_window, "#version 330");
					UI::GUI::Style(UI::GUI::Parameters::Style::Dark);
					
					// Set the delta time of the physics simulation.
					Time::FixedDeltaTime(1.0 / 60.0);
					
					tick_t physics_step = 0.0;
					
					// Load a scene and run:
					const auto scene = ECS::Scene::Load("levels/fyp.scene", _initialisers);
					scene->Begin();
					
					Utils:: ALDumpError();
					Utils:: GLDumpError();
					Utils::SDLDumpError();
					
					/* LOOP */
					while (!Application::s_Quit) {
						
						try {
							
							// Get the beginning of the frame for timing purposes.
							const auto frame_start = std::chrono::high_resolution_clock::now();
							
							// Configure V-sync:
							// -1 = Adaptive
							//  0 = Disable
							//  1 = Enable
							{
								const auto selected = Settings::Graphics::VSync::s_CurrentSelection - 1;
								
								if (selected != SDL_GL_GetSwapInterval()) { SDL_GL_SetSwapInterval(selected); }
							}
							
							/* INPUT */
							Input::Input::Tick();
							
							// Handle events:
							{
								// Process window resize event:
								if (const auto items = Input::Input::Event::Get(SDL_WINDOWEVENT)) {
									
									for (const auto& item : *items) {
										
										if (const auto w = Window::Get(static_cast<size_t>(item.window.windowID)).lock()) {
											
											if (item.window.event == SDL_WINDOWEVENT_RESIZED) {
												
												// Dirty the window.
												w->SetDirty();
												
												// Raise the reinitialisation flag.
												renderFlags = static_cast<Graphics::Camera::RenderFlags>(
													renderFlags | Graphics::Camera::RenderFlags::REINITIALISE);
											}
										}
									}
								}
								
								// Process quit event:
								if (Input::Input::Event::Get(SDL_QUIT)) {
									
									Application::Quit();
									
									goto NestedBreak;
								}
							}
							
							/* UPDATE CURSOR STATE */
							Input::Cursor::Update();
							
							/* FIXED UPDATE */
							
							// Runs as many times as needed to restore the physics step below zero.
							while (physics_step >= 0.0) {
								
								// Tick the physics engine.
								Physics::Physics::Tick(Time::FixedDeltaTime<tick_t>());
								
								// Tick the scene's fixed update.
								scene->FixedTick();
								
								// Subtract delta time from the physics step.
								physics_step -= Time::FixedUnscaledDeltaTime<tick_t>();
							}
							
							/* UPDATE */
							scene->Tick(renderFlags);
							
							// Reset render flags after rendering.
							renderFlags = Graphics::Camera::RenderFlags::NONE;
							
							/* GUI UPDATE */
							UI::GUI::OnGUI(main_window);
							
							/* UPDATE WINDOWS */
							const auto& windows = Window::m_Windows.Values();
							for (const auto& window : windows) {
								window->Update();
							}
							
							Utils::ALDumpError();
							Utils::GLDumpError();
							Utils::SDLDumpError();
							
							/* UPDATE TIMERS */
							
							Time::s_UnscaledDeltaTime =
								std::chrono::duration_cast<std::chrono::duration<tick_t>>(
									std::chrono::high_resolution_clock::now() -
									frame_start
								).count(); // Calculate delta time.
							
							            Time::s_Elapsed  += Time::UnscaledDeltaTime<tick_t>(); // Increment total elapsed time.
							Physics::Physics::s_LastTick += Time::UnscaledDeltaTime<tick_t>(); // Increment time since last physics update.
							                physics_step += Time::UnscaledDeltaTime<tick_t>(); // Increment the physics step (used for computing number of fixed updates per frame).
						}
						catch (const std::exception& e) {
							Debug::Log(e, LogType::Critical);
						}
					}
				}
				catch (const std::exception& e) {
					Debug::Log(e, LogType::Critical);
				}
	
	NestedBreak:
				
				/* FINALISE */
				Finalise();
				
				Debug::Log("Application Terminated Normally.", LogType::Info);
			}
			
			return 0;
		}
		
		/**
		 * @brief Quit function.
		 *
		 * This function is responsible for quitting the application.
		 *
		 * @see Application::s_Quit
		 * @see Debug::Log(const std::string_view&, const LogType&, const bool&)
		 */
		static void Quit() noexcept {
		
			Debug::Log("Application::Quit()", LogType::Info);
			
			Application::s_Quit = true;
		}
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_APPLICATION_H