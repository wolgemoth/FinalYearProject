#ifndef FINALYEARPROJECT_WINDOW_H
#define FINALYEARPROJECT_WINDOW_H

#include "../core/utils/Hashmap.h"
#include "../graphics/Camera.h"
#include "../graphics/Texture.h"

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

namespace LouiEriksson::Engine::Graphics {
	
	class Camera;
	
} // LouiEriksson::Engine::Graphics

namespace LouiEriksson::Engine {

	class Window final {
	
		friend class Application;
		
		friend Graphics::Camera;
	
	private:
		
		inline static Hashmap<int, std::shared_ptr<Window>> m_Windows;
	
		size_t m_ID;
		SDL_GLContext m_Context;
	
		std::shared_ptr<SDL_Window> m_Window;
	
		Hashmap<size_t, std::reference_wrapper<Graphics::Camera>> m_Cameras;
	
		void Link(Graphics::Camera& _camera) {
		
			if (m_Cameras.Add(reinterpret_cast<size_t>(&_camera), std::reference_wrapper(_camera))) {
				_camera.m_Window = Get(ID());
			}
			else {
				Debug::Log("Failed to link camera to window.", LogType::Error);
			}
		}
		
		void Unlink(Graphics::Camera& _camera) noexcept {
			_camera.m_Window.reset();
			m_Cameras.Remove(reinterpret_cast<size_t>(&_camera));
		}
	
		Window(const int& _width, const int& _height, const char* _name, bool _fullscreen = false, bool _hdr10 = true)  {
		
			m_Window = std::shared_ptr<SDL_Window>(
				SDL_CreateWindow(_name,
					SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
					_width, _height, (unsigned)SDL_WINDOW_RESIZABLE | (unsigned)SDL_WINDOW_ALLOW_HIGHDPI | (unsigned)SDL_WINDOW_OPENGL
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
	
	public:
		
		Window             (const Window& _other) = delete;
		Window& operator = (const Window& _other) = delete;
	
		static std::shared_ptr<Window> Create(const int& _width, const int& _height, const char* _name)  {
		
			std::shared_ptr<Window> result(new Window(_width, _height, _name), [](Window* _ptr) { delete _ptr; });
		
			m_Windows.Add(result->ID(), result);
		
			return result;
		}
	
		static std::weak_ptr<Window> Get(const size_t& _id) {
		
			auto result = m_Windows.Get(_id).value_or({});
			
			Debug::Assert(
				result != nullptr,
				"Failed getting window with ID: \"" + std::to_string(_id) + "\"",
				LogType::Error
			);
			
			return result;
		}
	
		static std::weak_ptr<Window> GetFocused()  {
			
			std::weak_ptr<Window> result;
			
			auto* const focused = SDL_GetKeyboardFocus();
			
			if (focused != nullptr) {
				 result = Get(static_cast<size_t>(SDL_GetWindowID(focused)));
			}
			
			return result;
		}
		
		inline static void Destroy(const size_t& _id) {
		
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
	
		[[nodiscard]] constexpr const size_t&      ID() const noexcept { return m_ID;      }
		[[nodiscard]] constexpr SDL_GLContext Context() const noexcept { return m_Context; }
		
		inline void Update() const {
			SDL_GL_SwapWindow(m_Window.get());
		}
		
		inline void Dimensions(const int& _width, const int& _height)  {
			SDL_SetWindowSize(m_Window.get(), _width, _height);
		
			SetDirty();
		}
		
		/**
		 * @brief Returns the window's x,y dimensions as a c-style array.
		 *
		 * @return glm::ivec2 The window dimensions as a glm::ivec2 object.
		 */
		[[nodiscard]] inline glm::ivec2 Dimensions() const  {
		
			glm::ivec2 result(-1, -1);
		
			SDL_GetWindowSize(m_Window.get(), &result[0], &result[1]);
		
			return result;
		}
	
		[[nodiscard]] inline float Aspect() const {
			const auto dimensions = Dimensions();
		
			return static_cast<float>(dimensions[0]) / static_cast<float>(dimensions[1]);
		}
	
		[[nodiscard]] inline bool Focused() const {
			return GetFlag(SDL_WINDOW_INPUT_FOCUS);
		}
		
		[[nodiscard]] inline Uint32 GetFlags() const {
			return SDL_GetWindowFlags(m_Window.get());
		}
		
		[[nodiscard]] inline bool GetFlag(const Uint32& _flag) const {
			
			auto flags = GetFlags();
			
			return (flags & _flag) != 0u;
		}
		
		inline void SetDirty() {
			
			const auto cameras = m_Cameras.Values();
		
			for (auto camera : cameras) {
				camera.get().SetDirty();
			}
		
			const auto dimensions = Dimensions();
			glViewport(0, 0, dimensions[0], dimensions[1]);
		}
	
		inline operator SDL_Window*() const { return m_Window.get(); }
		
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_WINDOW_H