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
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// TEMP:
#include "../spatial/osm/OSM.h"
#include "../spatial/elevation/Elevation.h"
#include "../spatial/maths/Coords.h"

namespace LouiEriksson::Engine {
	
	int Application::Main(const Hashmap<std::string, std::shared_ptr<Script> (*)(const std::weak_ptr<ECS::GameObject>& _parent)>& _initialisers) {
		
		// Restrict Main() to one instance.
		if (!s_Initialised) {
			 s_Initialised = true;
			
			srand(0u); // Use a constant seed (like '0') for deterministic behaviour.
			
			/* CREATE A MAIN WINDOW */
			auto main_window = Window::Create(1280, 720, "FinalYearProject");
			
			/* INIT RENDER FLAGS */
			auto renderFlags = Graphics::Camera::RenderFlags::REINITIALISE;
			
			/* INIT CURSOR STATE */
			Input::Cursor::SetState({ Input::Cursor::State::LockMode::Absolute, true });
			
			// Capture the mouse on startup.
			SDL_CaptureMouse(SDL_TRUE);
			
			/* INIT GLEW */
			if (glewInit() != GLEW_OK) {
				
				// Throw error if GLEW fails to initialise correctly.
				std::cout << "Failed to initialize GLEW!\n";
			}
			
			/* INIT SOUND */
			Audio::Sound::Init();
			
			/* INIT NETWORKING */
			Networking::Requests::Init();
			
			/* INIT RESOURCES */
			Resources::Init();
			
			/* INIT SETTINGS */
			Settings::Init();
			
			/* INIT PHYSICS */
			Physics::Physics::Init();
			
			/* INIT GUI */
			UI::GUI::Init(main_window, "#version 330");
			UI::GUI::Style(UI::GUI::Parameters::Style::Dark);
			
			// Set the delta time of the physics simulation.
			Time::FixedDeltaTime(1.0f / 60.0f);
			
			float physics_step = 0.0f;
			
			{
				auto bounds = Spatial::Maths::Coords::GPS::GPSToBounds({22.2829, 114.1736, 0.0}, 0.05);
				
//				auto task1 = Spatial::OSM::QueryOverpassBoundingBoxAsync(
//					bounds,
//					10U,
//					[](const Networking::Requests::Response& _response) {
//						std::cout << _response.Content().ToStream().str() << std::endl;
//					}
//				);
				
//				auto task2 = Spatial::Elevation::LoadElevationAsync(
//					bounds,
//					Spatial::Elevation::ElevationProvider::OpenElevation,
//					{ 2, 2 },
//					[](const std::vector<float>& _values) {
//
//						std::stringstream ss;
//
//						for (const auto& item : _values) {
//							ss << item << '\n';
//						}
//
//						std::cout << ss.str() << std::endl;
//					}
//				);
			}
			
			// Load a scene and run:
			auto scene = ECS::Scene::Load("levels/gep.scene", _initialisers);
			scene->Begin();
			
			/* LOOP */
			while (!Application::s_Quit) {
				
				try {
					
					// Get the beginning of the frame for timing purposes.
					const auto frame_start = std::chrono::high_resolution_clock::now();
					
					// Clear the AL error state by dumping the error at the start of the frame.
					Utils::GLDumpError(true);
					
					// Clear the GL error state by dumping the error at the start of the frame.
					Utils::GLDumpError(true);
					
					// Update (apply) the cursor's state.
					Input::Cursor::Update();
					
					// Configure V-sync:
					// -1 = Adaptive
					//  0 = Disable
					//  1 = Enable
					{
						auto selected = Settings::Graphics::VSync::s_CurrentSelection - 1;
						
						if (selected != SDL_GL_GetSwapInterval()) { SDL_GL_SetSwapInterval(selected); }
					}
					
					/* INPUT */
					Input::Input::Tick();
					
					// Handle events:
					{
						std::vector<SDL_Event> items;
						
						// Process window resize event:
						if (Input::Input::Event::Get(SDL_WINDOWEVENT, items)) {
							
							for (auto item : items) {
								
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
					auto windows = Window::m_Windows.Values();
					for (const auto& window : windows) {
						window->Update();
					}
					
					// Dump latest AL error log to the console.
					Utils::ALDumpError();
					
					// Dump latest GL error log to the console.
					Utils::GLDumpError();
				
					/* UPDATE TIMERS */
					Time::s_UnscaledDeltaTime = static_cast<float>(
						std::chrono::duration_cast<std::chrono::microseconds>(
							std::chrono::high_resolution_clock::now() -
							frame_start
						).count() / 1000000.0); // Calculate delta time.
		
					            Time::s_Elapsed  += Time::UnscaledDeltaTime(); // Increment total elapsed time.
					Physics::Physics::s_LastTick += Time::UnscaledDeltaTime(); // Increment time since last physics update.
					                physics_step += Time::UnscaledDeltaTime(); // Increment the physics step (used for computing number of fixed updates per frame).
				}
				catch (const std::exception& e) {
					std::cerr << e.what() << std::endl;
				}
			}
			
NestedBreak:
			
			/* FINALISE */
			Input::Cursor::Reset();
			
			      UI::     GUI::Dispose();
			 Physics:: Physics::Dispose();
			   Input::   Input::Dispose();
			   Audio::   Sound::Dispose();
		  Networking::Requests::Dispose();
			
			SDL_Quit();
		}
		else {
			std::cout << "Attempted to call Application::Main() while it is already running! Do you have multiple instances?\n";
		}
	
		return 0;
	}
	
	void Application::Quit() noexcept {
		Application::s_Quit = true;
	}
	
} // LouiEriksson::Engine