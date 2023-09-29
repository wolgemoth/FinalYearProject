#pragma once

#ifndef _WINDOW_H
#define _WINDOW_H

#include "Hashmap.h"
#include "Camera.h"

class Window {

	friend class Application;
	friend class Camera;

private:
	
	inline static Hashmap<int, std::shared_ptr<Window>> m_Windows;

	int m_ID;

	SDL_Window* m_Window;

	Hashmap<int, std::reference_wrapper<Camera>> m_Cameras;

	void   Link(Camera& _camera);
	void Unlink(Camera& _camera);

	Window(const Window& _other) = delete;
	Window& operator = (const Window& _other) = delete;

	Window(int _width, int _height, const char* _name);
	~Window();

public:

	static std::shared_ptr<Window> Create(int _width, int _height, const char* _name);

	static std::shared_ptr<Window> Get(int _id);

	static void Destroy(int _id);

	int ID();

	void Update();

	void Dimensions(int _width, int _height);
	int* Dimensions();

	float Aspect();

	void SetDirty();

	operator SDL_Window*() { return m_Window; }
};

#endif // !_WINDOW_H
