#pragma once

#ifndef FINALYEARPROJECT_APPLICATION_H
#define FINALYEARPROJECT_APPLICATION_H

#include "Time.h"
#include "Input.h"
#include "Scene.h"
#include "Resources.h"

namespace LouiEriksson {
	
	/// <summary> Main class for the application. </summary>
	class Application {
	
	private:
	
		inline static bool  s_Quit        = false;
		inline static bool  s_Initialised = false;
	
	public:
	
		 Application()                          = delete;
		 Application(const Application& _other) = delete;
		~Application()                          = delete;
		
		Application& operator = (const Application&  _other) = delete;
		Application& operator =       (Application&& _other) = delete;
	
		/// <summary> Quits the application. </summary>
		static void Quit();
	
		/// <summary> Main function of the application. </summary>
		static int Main();
	};
}

#endif //FINALYEARPROJECT_APPLICATION_H