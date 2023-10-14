#include "stdafx.h"

#include "Window.h"

namespace LouiEriksson {
	
	Window::Window(const int& _width, const int& _height, const char* _name) {
	
		m_Window = std::shared_ptr<SDL_Window>(
			SDL_CreateWindow(_name,
				SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				_width, _height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
			),
			[](SDL_Window* _ptr) { SDL_DestroyWindow(_ptr); } // Custom deleter calls SDL_DestroyWindow();
		);
	
		m_ID = SDL_GetWindowID(m_Window.get());
	
		if (SDL_GL_CreateContext(m_Window.get()) == NULL) { // NOLINT(*-use-nullptr)
	
			std::stringstream err;
			err << "ERROR (Window.cpp [Window(int, int, const char*)]): " <<
				SDL_GetError();
	
			std::cout << err.str() << "\n";
	
			throw std::runtime_error(err.str());
		}
	}
	
	std::shared_ptr<Window> Window::Create(const int& _width, const int& _height, const char* _name) {
	
		std::shared_ptr<Window> result(new Window(_width, _height, _name), [](Window* _ptr) { delete _ptr; });
	
		m_Windows.Add(result->ID(), result);
	
		return result;
	}
	
	std::shared_ptr<Window> Window::Get(const int& _id) {
	
		std::shared_ptr<Window> result(nullptr);
	
		if (!m_Windows.Get(_id, result)) {
	
			std::stringstream err;
			err << "ERROR (Application.cpp [GetWindow(int)]): Failed getting window with ID: \"" <<
				_id << "\"";
	
			throw std::runtime_error(err.str());
		}
	
		return result;
	}
	
	void Window::Destroy(const int& _id) {
	
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
	
	const int& Window::ID() const {
		return m_ID;
	}
	
	void Window::Update() const {
		SDL_GL_SwapWindow(m_Window.get());
	}
	
	void Window::Dimensions(const int& _width, const int& _height) {
		SDL_SetWindowSize(m_Window.get(), _width, _height);
	
		SetDirty();
	}
	
	#pragma warning(disable : 4172)
	
	/// <summary>
	/// Returns the window's x,y dimensions as a c-style array.
	/// <remarks>The size of the returned array is 2.</remarks>
	/// </summary>
	glm::ivec2 Window::Dimensions() const {
	
		glm::ivec2 result(-1, -1);
	
		SDL_GetWindowSize(m_Window.get(), &result[0], &result[1]);
	
		return result;
	}
	
	#pragma warning(default : 4172)
	
	float Window::Aspect() const {
		const auto dimensions = Dimensions();
	
		return static_cast<float>(dimensions[0]) / static_cast<float>(dimensions[1]);
	}
	
	void Window::Link(Camera& _camera) {
		_camera.m_Window = Window::Get(ID());
		m_Cameras.Add((int)(size_t)&_camera, std::reference_wrapper(_camera));
	}
	
	void Window::Unlink(Camera& _camera) {
		_camera.m_Window = std::shared_ptr<Window>(nullptr);
		m_Cameras.Remove((int)(size_t)&_camera);
	}
	
	void Window::SetDirty() {
		
		const auto cameras = m_Cameras.Values();
	
		for (auto camera : cameras) {
			camera.get().SetDirty();
		}
	
		const auto dimensions = Dimensions();
		glViewport(0, 0, dimensions[0], dimensions[1]);
	}
}