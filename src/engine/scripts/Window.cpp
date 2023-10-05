#include "stdafx.h"

#include "Window.h"

namespace LouiEriksson {
	
	Window::Window(int _width, int _height, const char* _name) {
	
		m_Window = SDL_CreateWindow(_name,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			_width, _height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	
		m_ID = SDL_GetWindowID(m_Window);
	
		if (SDL_GL_CreateContext(m_Window) == NULL) {
			throw std::runtime_error("ERROR (Window.cpp [Window(int, int, const char*)]): Failed to create OpenGL context.");
		}
	}
	
	Window::~Window() {
		SDL_DestroyWindow(m_Window);
	}
	
	std::shared_ptr<Window> Window::Create(int _width, int _height, const char* _name) {
		std::shared_ptr<Window> result(new Window(_width, _height, _name), [](Window* _ptr) { delete _ptr; });
	
		m_Windows.Add(result->ID(), result);
	
		return result;
	}
	
	std::shared_ptr<Window> Window::Get(uint _id) {
	
		std::shared_ptr<Window> result(nullptr);
	
		if (!m_Windows.Get(_id, result)) {
	
			std::stringstream err;
			err << "ERROR (Application.cpp [GetWindow(int)]): Failed getting window with ID: \"" <<
				_id << "\"";
	
			throw std::runtime_error(err.str());
		}
	
		return result;
	}
	
	void Window::Destroy(int _id) {
	
		std::shared_ptr<Window> window(Get(_id));
	
		if (window != nullptr) {
	
			if (m_Windows.Remove(window->ID())) {
	
				// Unlink window from cameras.
				for (auto camera : window->m_Cameras.Values()) {
					camera.get().m_Window = std::shared_ptr<Window>(nullptr);
				}
	
				// Unlink cameras from window.
				window->m_Cameras.Clear();
	
				// "Destroy"
				window.reset();
			}
			else {
				std::stringstream err;
				err << "ERROR (Application.cpp [DestroyWindow(int)]): Failed to destroy window with ID: \"" <<
					_id << "\"";
	
				throw std::runtime_error(err.str());
			}
		}
	}
	
	uint Window::ID() const {
		return m_ID;
	}
	
	void Window::Update() {
		SDL_GL_SwapWindow(m_Window);
	}
	
	void Window::Dimensions(int _width, int _height) {
		SDL_SetWindowSize(m_Window, _width, _height);
	
		SetDirty();
	}
	
	/// <summary>
	/// Returns the window's x,y dimensions as a c-style array.
	/// <remarks>The size of the returned array is 2.</remarks>
	/// </summary>
	glm::ivec2 Window::Dimensions() {
		
		glm::ivec2 result { -1, -1 };
	
		SDL_GetWindowSize(m_Window, &result.x, &result.y);
	
		return result;
	}
	
	float Window::Aspect() {
		auto dimensions = Dimensions();
	
		return (float)dimensions.x / (float)dimensions.y;
	}
	
	void Window::Link(Camera& _camera) {
		_camera.m_Window = Window::Get(ID());
		m_Cameras.Add((size_t)&_camera, std::reference_wrapper<Camera>(_camera));
	}
	
	void Window::Unlink(Camera& _camera) {
		_camera.m_Window = std::shared_ptr<Window>(nullptr);
		m_Cameras.Remove((size_t)&_camera);
	}
	
	void Window::SetDirty() {
		
		auto cameras = m_Cameras.Values();
	
		for (auto camera : cameras) {
			camera.get().SetDirty();
		}
		
		auto dimensions = Dimensions();
		glViewport(0, 0, dimensions.x, dimensions.y);
	}
}