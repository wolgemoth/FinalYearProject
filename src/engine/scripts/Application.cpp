#include "stdafx.h"

#include "Application.h"

namespace LouiEriksson {
	
	void Application::Quit() {
		Application::s_Quit = true;
	}
	
	int Application::Main() {
		
		if (!s_Initialised) {
			s_Initialised = true;
			
			srand(0); // Use a constant seed (like '0') for deterministic behaviour.
			
			/* INIT */
			Window::Create(1280, 720, "FinalYearProject");
			
			SDL_GL_SetSwapInterval(-1); // 0 = Disable vsync. 1 = Enable vsync. -1 = Adaptive
	
			if (glewInit() != GLEW_OK) {
				throw std::runtime_error("ERROR (Application.cpp [Main()]): Failed to initialize GLEW!");
			}
			
			Shader::PreloadShaders();
			
			auto scene = Scene::Load("levels/pfg.scene");
			scene->Begin();
	
			auto light_gameObject = GameObject::Create(scene->shared_from_this(), "Light");
			scene->Attach(light_gameObject);
			
			// TODO: Temporary code for debugging. Please remove me!
			{
				// Add a light to the scene for testing purposes.
				auto light_transform = light_gameObject->AddComponent<Transform>();
				
				light_transform->m_Position = glm::vec3(0, 1.5f, 0);
				light_transform->m_Rotation = glm::quat(glm::radians(glm::vec3(-45, 135, 0)));
				
				auto light = scene->Attach(light_gameObject->AddComponent<Light>());
			}
			
			// Set the delta time of the physics simulation.
			Time::FixedDeltaTime(0.02f);
	
			float physics_step = 0.0f;
			float fps_timer    = 0.0f;
	
			/* LOOP */
			while (!Application::s_Quit) {
				
				try {
				
					auto frame_start = std::chrono::high_resolution_clock::now();
					
					SDL_Event event = { 0 };
					
					while (SDL_PollEvent(&event) != 0) {
						
						if (event.type == SDL_WINDOWEVENT) {
							
							if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
								Window::Get(event.window.windowID)->SetDirty();
							}
						}
						else if (event.type == SDL_QUIT) {
							Application::Quit();
							
							goto NestedBreak;
						}
					}
					
					// Get input.
					SDL_PumpEvents();
					Input::KeyboardState(SDL_GetKeyboardState(NULL)); // NOLINT(*-use-nullptr)
					
					scene->Tick();
		
					if (physics_step <= 0.0f) {
						physics_step = Time::FixedDeltaTime();
		
						scene->FixedTick();
					}
					
					auto windows = Window::m_Windows.Values();
					for (const auto& window : windows) {
						window->Update();
					}
					
					// Update the delta time.
					Time::s_DeltaTime =
						(float)std::chrono::duration_cast<std::chrono::microseconds>(
							std::chrono::high_resolution_clock::now() -
							frame_start
						).count() / 1000000.0f;
		
					// Update the elapsed time.
					Time::s_Elapsed += Time::DeltaTime();
		
					/* DEBUGGING */
					if (fps_timer <= 0.0f) {
						fps_timer = 1.0f;
		
						std::vector<std::any> entities;
						scene->Entities().Get(typeid(Rigidbody), entities);
		
						std::cout << "\n~ Performance Metrics ~\n" <<
							"Rigidbodies:\t" << entities.size() << "\n"
							"FPS:\t\t\t" << 1.0f / Time::s_DeltaTime << "\n";
					}
		
					physics_step -= Time::DeltaTime();
					fps_timer    -= Time::DeltaTime();
					
				}
				catch (const std::exception& e) {
					std::cout << e.what() << "\n";
				}
			}
	
NestedBreak:
			
			/* DISPOSE */
			SDL_Quit();
		}
		else {
			throw(std::runtime_error("ERROR (Application.cpp [Main()]): Attempted to call Main() while it is already running!"));
		}
	
		return 0;
	}
}