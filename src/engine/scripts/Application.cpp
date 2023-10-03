#include "stdafx.h"

#include "Application.h"

namespace LouiEriksson {
	
	void Application::Quit() {
		Application::s_Quit = true;
	}
	
	int Application::Main() {
	
		srand(0); // Use a constant seed (like '0') for deterministic behaviour.
	
		if (!s_Initialised) {
			s_Initialised = true;
	
			/* INIT */
			Window::Create(640, 480, "My Game");
	
			if (glewInit() != GLEW_OK) {
				throw std::runtime_error("ERROR (Application.cpp [Main()]): Failed to initialize GLEW!");
			}
	
			auto scene = Scene::Load("levels/pfg.scene");
			scene->Begin();
	
			// Set the delta time of the physics simulation.
			Time::FixedDeltaTime(1.0f / 60.0f);
	
			float physics_step = 0.0f;
			float fps_timer    = 0.0f;
	
			/* LOOP */
			while (!Application::s_Quit) {
	
				auto frame_start = std::chrono::high_resolution_clock::now();
	
				SDL_Event event = { 0 };
	
				while (SDL_PollEvent(&event)) {
	
					if (event.type == SDL_QUIT) {
						Application::Quit();
	
						goto NestedBreak;
					}
					else if (event.type == SDL_WINDOWEVENT) {
	
						if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
							Window::Get(event.window.windowID)->SetDirty();
						}
					}
				}
	
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
						"FPS:\t\t" << 1.0f / Time::s_DeltaTime << "\n";
				}
	
				physics_step -= Time::DeltaTime();
				fps_timer    -= Time::DeltaTime();
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