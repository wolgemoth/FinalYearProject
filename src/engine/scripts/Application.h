#pragma once

#ifndef FINALYEARPROJECT_APPLICATION_H
#define FINALYEARPROJECT_APPLICATION_H

#include "Time.h"
#include "Input.h"
#include "Scene.h"

namespace LouiEriksson {
	
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
	
		static void Quit();
	
		static int Main();
	};
}

#endif //FINALYEARPROJECT_APPLICATION_H