#include "stdafx.h"

#include "Application.h"

#include "Cursor.h"
#include "GUI.h"
#include "Input.h"
#include "Settings.h"
#include "Sound.h"
#include "Physics.h"
#include "Resources.h"

namespace LouiEriksson {
	
	int Application::Main() {
		
		// Restrict Main() to one instance.
		if (!s_Initialised) {
			s_Initialised = true;
			
			srand(0); // Use a constant seed (like '0') for deterministic behaviour.
			
			/* CREATE A MAIN WINDOW */
			auto main_window = Window::Create(1280, 720, "FinalYearProject");
			
			/* INIT CURSOR STATE */
			
			// Init the cursor's state.
			Cursor::State state = { main_window, Cursor::State::LockMode::Absolute, true };
			Cursor::SetState(state);
			
			/* INIT GLEW */
			if (glewInit() != GLEW_OK) {
				
				// Throw error if GLEW fails to initialise correctly.
				throw std::runtime_error("ERROR (Application.cpp [Main()]): Failed to initialize GLEW!");
			}
			
			/* INIT SOUND */
			Sound::Init();
			
			/* PRELOAD RESOURCES */
			Resources::Preload();
			
			/* INIT SETTINGS */
			Settings::Init();
			
			/* INIT PHYSICS */
			Physics::Init();
			
			/* INIT GUI */
			GUI::Init(main_window, "#version 330");
			GUI::Style(GUI::Parameters::Style::Dark);
			
			// Load a scene and run:
			auto scene = Scene::Load("levels/gep.scene");
			scene->Begin();
	
			auto light_gameObject = GameObject::Create(scene->shared_from_this(), "Light");
			scene->Attach(light_gameObject);
			
			// TODO: Temporary code for debugging. Please remove me!
			{
				// Add a light to the scene for testing purposes.
				auto light_transform = light_gameObject->AddComponent<Transform>();
				auto light = scene->Attach(light_gameObject->AddComponent<Light>());
			}
			
			// Set the delta time of the physics simulation.
			Time::FixedDeltaTime(1.0f / 60.0f);
	
			float physics_step = 0.0f;
			
			/* LOOP */
			while (!Application::s_Quit) {
				
				try {
				
					// Clear the AL error state by dumping the error at the start of the frame.
					Utils::GLDumpError(true);
					
					// Clear the GL error state by dumping the error at the start of the frame.
					Utils::GLDumpError(true);
					
					// Get the beginning of the frame for timing purposes.
					auto frame_start = std::chrono::high_resolution_clock::now();
				
					// Configure V-sync:
					// -1 = Adaptive
					//  0 = Disable
					//  1 = Enable
					{
						auto selected = Settings::Graphics::VSync::s_CurrentSelection - 1;
						
						if (selected != SDL_GL_GetSwapInterval()) { SDL_GL_SetSwapInterval(selected); }
					}
					
					/* INPUT */
					Input::Tick();
					
					// Handle events:
					{
						std::vector<SDL_Event> items;
						
						// Process window resize event:
						if (Input::Event::Get(SDL_WINDOWEVENT, items)) {
							
							for (auto item : items) {
								
								const auto window = Window::Get(static_cast<int>(item.window.windowID));
								
								if (item.window.event == SDL_WINDOWEVENT_RESIZED) {
									window->SetDirty();
								}
								else if (item.window.event == SDL_WINDOWEVENT_HIDDEN) {
									
									std::cout << "Lost focus of window \"" << window->m_ID << "\"\n";
									
									/*
									 * Set the last_focused_state value to the cursor's
									 * current state, and invalidate the window.
									 */
									state = Cursor::GetState();
									state.m_Window.reset();
								
									// Reset the cursor's state.
									Cursor::Reset();
								}
								else if (item.window.event == SDL_WINDOWEVENT_SHOWN) {
									
									std::cout << "Gained focus of window \"" << window->m_ID << "\"\n";
									
									/*
									 * Update the window of the last_focused_state value
									 * and set it as the cursor's current state.
									 */
									state.m_Window = window;
									
									Cursor::SetState(state);
								}
							}
						}
						
						// Process quit event:
						if (Input::Event::Get(SDL_QUIT)) {
							
							Application::Quit();
	
							goto NestedBreak;
						}
					}
					
					/* UPDATE CURSOR STATE */
					Cursor::Update();
					
					/* FIXED UPDATE */
					
					// Runs as many times as needed to restore the physics step below zero.
					while (physics_step >= 0.0f) {
						
						Physics::Tick(Time::FixedDeltaTime());
						
						scene->FixedTick();
						
						physics_step -= Time::FixedDeltaTime();
					}
					
					/* UPDATE */
					scene->Tick();
		
					/* GUI UPDATE */
					GUI::OnGUI(main_window);
					
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
					Time::s_DeltaTime =
						static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(
							std::chrono::high_resolution_clock::now() -
							frame_start
						).count()) / 1000000.0f; // Calculate delta time.
		
					   Time::s_Elapsed  += Time::DeltaTime(); // Increment total elapsed time.
					Physics::s_LastTick += Time::DeltaTime(); // Increment time since last physics update.
					       physics_step += Time::DeltaTime(); // Increment the physics step (used for computing number of fixed updates per frame).
				}
				catch (const std::exception& e) {
					std::cout << e.what()<< '\n';
				}
			}
			
NestedBreak:
			
			/* FINALISE */
			Cursor::Reset();
			
			    GUI::Dispose();
			Physics::Dispose();
			
			SDL_Quit();
		}
		else {
			throw(std::runtime_error("ERROR (Application.cpp [Main()]): Attempted to call Main() while it is already running!"));
		}
	
		return 0;
	}
	
	void Application::Quit() {
		Application::s_Quit = true;
	}
	
}