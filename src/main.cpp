#include "engine/scripts/core/Application.h" // Engine runtime.
#include "usr/core/Injector.h"               // User scripts.

/// <summary>
/// Entry point for the program.
/// </summary>
int main([[maybe_unused]] int _argc, [[maybe_unused]] char* _argv[]) {
	
	/*
	 * 	Create an instance of Application and run.
	 * 	Inject the function pointers necessary for initialisation of custom scripts.
	 */
	return LouiEriksson::Application::Main(LouiEriksson::Scripting::Injector::GetInitialisers());
	
} // LouiEriksson