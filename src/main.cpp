
#include "engine/scripts/core/Application.hpp" // Engine runtime.
#include "game/Injector.hpp"                   // User scripts.

/**
 * @file main.cpp
 * @brief Global entry point.
 */
int main([[maybe_unused]] int _argc, [[maybe_unused]] char* _argv[]) {
	
	/*
	 * 	Create an instance of Application and run.
	 * 	Inject the function pointers necessary for initialisation of custom scripts.
	 */
	return LouiEriksson::Engine::Application::Main(LouiEriksson::Game::Core::Injector::GetInitialisers());
	
}