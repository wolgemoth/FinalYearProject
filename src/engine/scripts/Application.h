#pragma once

#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "Time.h"

#include "Scene.h"

class Application {

private:

	inline static bool  s_Quit        = false;
	inline static bool  s_Initialised = false;

	 Application() = delete;
	 Application(const Application& _other) = delete;
	~Application() = delete;

	Application& operator = (const Application& _other) = delete;
	Application& operator = (Application&& _other) = delete;

public:

	static void Quit();

	static int Main();
};

#endif // !_APPLICATION_H
