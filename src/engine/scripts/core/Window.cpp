#include "Window.h"

#include "../graphics/Camera.h"
#include "Debug.h"

#include <SDL_error.h>
#include <SDL_pixels.h>
#include <SDL_stdinc.h>
#include <SDL_video.h>
#include <SDL_keyboard.h>

#include <GL/glew.h>
#include <glm/ext/vector_int2.hpp>

#include <cstddef>
#include <functional>
#include <memory>
#include <string>

namespace LouiEriksson::Engine {
	
	Window::Window(const int& _width, const int& _height, const char* _name, bool _fullscreen, bool _hdr10) {
	
		m_Window = std::shared_ptr<SDL_Window>(
			SDL_CreateWindow(_name,
				SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				_width, _height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL
			),
			[](SDL_Window* _ptr) { SDL_DestroyWindow(_ptr); } // Custom deleter calls SDL_DestroyWindow();
		);
		
		if (_fullscreen) {
			SDL_SetWindowFullscreen(m_Window.get(), SDL_WINDOW_FULLSCREEN);
		}
		
		m_ID = static_cast<int>(SDL_GetWindowID(m_Window.get()));
	
		// Grab the display mode to further configure parameters.
	    SDL_DisplayMode displayMode;
	    SDL_GetCurrentDisplayMode(m_ID, &displayMode);
		
		// Set the display mode to 10bpp if the HDR10 flag is raised.
		displayMode.format = _hdr10 ?
				SDL_PIXELFORMAT_ARGB2101010 :
				SDL_PIXELFORMAT_RGB24;
		
		if (SDL_SetWindowDisplayMode(m_Window.get(), &displayMode) != 0) {
			Debug::Log(SDL_GetError(), LogType::Error);
		}
		else {
			Debug::Log(
				"Output Format: \"" + std::string(SDL_GetPixelFormatName(displayMode.format)) + "\" (" +
						std::to_string(SDL_BITSPERPIXEL(displayMode.format)) + "bpp)",
				LogType::Info
			);
		}
		
		m_Context = SDL_GL_CreateContext(m_Window.get());
		
		if (m_Context == nullptr) {
			Debug::Log("SDL failed to create GL context! " + std::string(SDL_GetError()), LogType::Error);
		}
	}
	
	std::shared_ptr<Window> Window::Create(const int& _width, const int& _height, const char* _name) {
	
		std::shared_ptr<Window> result(new Window(_width, _height, _name), [](Window* _ptr) { delete _ptr; });
	
		m_Windows.Add(result->ID(), result);
	
		return result;
	}
	
	std::weak_ptr<Window> Window::Get(const int& _id) {
	
		std::shared_ptr<Window> result;
		
		Debug::Assert(
			m_Windows.Get(_id, result),
			"Failed getting window with ID: \"" + std::to_string(_id) + "\"",
			LogType::Error
		);
		
		return result;
	}
	
	std::weak_ptr<Window> Window::GetFocused() {
		
		std::weak_ptr<Window> result;
		
		auto* const focused = SDL_GetKeyboardFocus();
		
		if (focused != nullptr) {
			 result = Get(SDL_GetWindowID(focused));
		}
		
		return result;
	}
	
	void Window::Destroy(const int& _id) {
	
		if (auto window = Get(_id).lock()) {
	
			if (m_Windows.Remove(window->ID())) {
	
				// Unlink window from cameras.
				for (auto camera : window->m_Cameras.Values()) {
					camera.get().m_Window.reset();
				}
	
				// Unlink cameras from window.
				window->m_Cameras.Clear();
	
				// "Destroy"
				window.reset();
			}
			else {
				Debug::Log("Failed to destroy window with ID: \"" + std::to_string(_id) + "\"", LogType::Error);
			}
		}
	}
	
	const int& Window::ID() const noexcept {
		return m_ID;
	}
	
	SDL_GLContext Window::Context() const noexcept {
		return m_Context;
	}
	
	void Window::Update() const {
		SDL_GL_SwapWindow(m_Window.get());
	}
	
	void Window::Dimensions(const int& _width, const int& _height) {
		SDL_SetWindowSize(m_Window.get(), _width, _height);
	
		SetDirty();
	}
	
	glm::ivec2 Window::Dimensions() const {
	
		glm::ivec2 result(-1, -1);
	
		SDL_GetWindowSize(m_Window.get(), &result[0], &result[1]);
	
		return result;
	}
	
	float Window::Aspect() const {
		const auto dimensions = Dimensions();
	
		return static_cast<float>(dimensions[0]) / static_cast<float>(dimensions[1]);
	}
	
	void Window::Link(Graphics::Camera& _camera) {
		
		if (m_Cameras.Add(reinterpret_cast<size_t>(&_camera), std::reference_wrapper(_camera))) {
			_camera.m_Window = Get(ID());
		}
		else {
			Debug::Log("Failed to link camera to window.", LogType::Error);
		}
	}
	
	void Window::Unlink(Graphics::Camera& _camera) noexcept {
		_camera.m_Window.reset();
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
	
	bool Window::Focused() const {
		return GetFlag(SDL_WINDOW_INPUT_FOCUS);
	}
	
	Uint32 Window::GetFlags() const {
		return SDL_GetWindowFlags(m_Window.get());
	}
	
	bool Window::GetFlag(const Uint32& _flag) const {
		
		auto flags = GetFlags();
		
		return (flags & _flag) != 0u;
	}
	
} // LouiEriksson::Engine