#include "Application.h"

#include "../audio/Sound.h"
#include "../core/Time.h"
#include "../core/utils/Utils.h"
#include "../ecs/Scene.h"
#include "../input/Cursor.h"
#include "../input/Input.h"
#include "../networking/Requests.h"
#include "../physics/Physics.h"
#include "../ui/GUI.h"

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

#include <chrono>
#include <cstdlib>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// TEMP:
#include "../spatial/maths/Coords.h"
#include "../spatial/elevation/Elevation.h"

namespace LouiEriksson::Engine {
	
	void Application::Finalise() {
		
		Debug::Log("Application::Finalise()", LogType::Info);
		
		Input::Cursor::Reset();
		
		        UI::     GUI::Dispose();
		   Physics:: Physics::Dispose();
		     Input::   Input::Dispose();
		     Audio::   Sound::Dispose();
		Networking::Requests::Dispose();
		
	    try {
			SDL_Quit();
		}
		catch (const std::exception& e) {
			Debug::Log(e, LogType::Critical);
		}
		
		Debug::Flush();
	}
	
	void Application::OnTerminate() {
		
		Debug::Log("Application::OnTerminate()", LogType::Critical);
		
		try {
			Utils::ALDumpError();
			Utils::GLDumpError();
			
			Finalise();
		}
		catch (const std::exception& e) {
			Debug::Log(e, LogType::Critical);
		}
		
		std::exit(1);
	}
	
	int Application::Main(const Hashmap<std::string, std::shared_ptr<Script> (*)(const std::weak_ptr<ECS::GameObject>& _parent)>& _initialisers) {
		
		// Restrict Main() to one instance.
		if (s_Initialised) {
			Debug::Log("Attempted to call Application::Main() while it is already running! Do you have multiple instances?", LogType::Warning);
		}
		else {
			s_Initialised = true;
			
			Debug::Log("Application Initialising.", LogType::Info);
		
			// Set custom termination behaviour:
			std::set_terminate(OnTerminate);
			
			try {
				
				srand(0u); // Use a constant seed (like '0') for deterministic behaviour.
				
				/* INIT */
				auto main_window = Window::Create(1280, 720, "FinalYearProject");
				
				if (glewInit() != GLEW_OK) {
					
					// Throw error if GLEW fails to initialise correctly.
					throw std::runtime_error("Failed to initialise GLEW!");
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
				Time::FixedDeltaTime(1.0f / 60.0f);
				
				float physics_step = 0.0f;
				
				// Load a scene and run:
				const auto scene = ECS::Scene::Load("levels/fyp.scene", _initialisers);
				scene->Begin();
				
				/* LOOP */
				while (!Application::s_Quit) {
					
					try {
						
						// Get the beginning of the frame for timing purposes.
						const auto frame_start = std::chrono::high_resolution_clock::now();
						
						Utils::ALDumpError();
						Utils::GLDumpError();
						
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
							std::vector<SDL_Event> items;
							
							// Process window resize event:
							if (Input::Input::Event::Get(SDL_WINDOWEVENT, items)) {
								
								for (const auto& item : items) {
									
									if (const auto w = Window::Get(static_cast<int>(item.window.windowID)).lock()) {
										
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
						while (physics_step >= 0.0f) {
							
							// Tick the physics engine.
							Physics::Physics::Tick(Time::FixedDeltaTime());
							
							// Tick the scene's fixed update.
							scene->FixedTick();
							
							// Subtract delta time from the physics step.
							physics_step -= Time::FixedUnscaledDeltaTime();
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
						
						/* UPDATE TIMERS */
						Time::s_UnscaledDeltaTime = static_cast<float>(
							std::chrono::duration_cast<std::chrono::microseconds>(
									std::chrono::high_resolution_clock::now() - frame_start
							).count() / 1000000.0); // Calculate delta time.
						
						            Time::s_Elapsed  += Time::UnscaledDeltaTime(); // Increment total elapsed time.
						Physics::Physics::s_LastTick += Time::UnscaledDeltaTime(); // Increment time since last physics update.
						                physics_step += Time::UnscaledDeltaTime(); // Increment the physics step (used for computing number of fixed updates per frame).
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
	
	void Application::Quit() noexcept {
		
		Debug::Log("Application::Quit()", LogType::Info);
		
		Application::s_Quit = true;
	}
	
} // LouiEriksson::Engine