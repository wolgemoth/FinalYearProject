#ifndef FINALYEARPROJECT_WINDOW_HPP
#define FINALYEARPROJECT_WINDOW_HPP

#include "../core/IViewport.hpp"
#include "../core/utils/Hashmap.hpp"
#include "../graphics/Camera.hpp"
#include "../graphics/Texture.hpp"
#include "Debug.hpp"

#include <SDL_error.h>
#include <SDL_keyboard.h>
#include <SDL_pixels.h>
#include <SDL_stdinc.h>
#include <SDL_video.h>

#include <GL/glew.h>

#include <cstddef>
#include <memory>

namespace LouiEriksson::Engine {

	class Window final : public IViewport<scalar_t, size_t>, public std::enable_shared_from_this<Window> {
	
		friend class Application;
		friend class GameObject;
	
	private:
		
		inline static Hashmap<size_t, std::shared_ptr<Window>> s_Windows;
	
		bool m_IsDirty;
		
		size_t m_ID;
		SDL_GLContext m_Context;
	
		std::shared_ptr<SDL_Window> m_Window;
		
		std::unordered_set<std::shared_ptr<Graphics::Camera>> m_Cameras;
		
		Window(const size_t& _width, const size_t& _height, const std::string_view& _name, bool _fullscreen = false, bool _hdr10 = true) : m_IsDirty(true) {
			
			m_Window = std::shared_ptr<SDL_Window>(
				SDL_CreateWindow(_name.data(),
					SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
					static_cast<int>(_width), static_cast<int>(_height),
					static_cast<unsigned>(SDL_WINDOW_RESIZABLE)         |
						static_cast<unsigned>(SDL_WINDOW_ALLOW_HIGHDPI) |
						static_cast<unsigned>(SDL_WINDOW_OPENGL)
				),
				[](SDL_Window* _ptr) { SDL_DestroyWindow(_ptr); } // Custom deleter calls SDL_DestroyWindow();
			);
			
			if (_fullscreen) {
				SDL_SetWindowFullscreen(m_Window.get(), SDL_WINDOW_FULLSCREEN);
			}
			
			m_ID = static_cast<size_t>(SDL_GetWindowID(m_Window.get()));
		
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
	
		~Window() noexcept {
			
			try {
				m_Cameras.clear();
			}
			catch (const std::exception& e) {
				Debug::Log(e, LogType::Critical);
			}
			
			m_Window.reset();
		}
		
	public:
		
		Window             (const Window& _other) = delete;
		Window& operator = (const Window& _other) = delete;
	
		static std::shared_ptr<Window> Create(const size_t& _width, const size_t& _height, const std::string_view& _name)  {
		
			std::shared_ptr<Window> result(new Window(_width, _height, _name), [](Window* _ptr) { delete _ptr; });
		
			s_Windows.Add(result->ID(), result);
		
			return result;
		}
	
		static std::weak_ptr<Window> Get(const size_t& _id) {
		
			std::weak_ptr<Window> result;
			
			if (const auto existing = s_Windows.Get(_id)) {
				result = existing.value();
			}
			else {
				Debug::Log("Failed getting window with ID: \"" + std::to_string(_id) + "\"", LogType::Error);
			}
			
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
		
		static void Destroy(const size_t& _id) {
		
			if (const auto window = Get(_id).lock()) {
		
				if (s_Windows.Remove(window->ID())) {
		
					// Unlink cameras from window.
					window->m_Cameras.clear();
				}
				else {
					Debug::Log("Failed to destroy window with ID: \"" + std::to_string(_id) + "\"", LogType::Error);
				}
			}
		}
	
		[[nodiscard]] constexpr const size_t&      ID() const noexcept { return m_ID;      }
		[[nodiscard]] constexpr SDL_GLContext Context() const noexcept { return m_Context; }
		
		void Attach(std::shared_ptr<Graphics::Camera> _camera) noexcept {
			
			try {
				m_Cameras.emplace(_camera);
				_camera->SetViewport(shared_from_this());
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
		}
		
		void Detach(std::shared_ptr<Graphics::Camera> _camera) noexcept {
			
			try {
				m_Cameras.erase(_camera);
				_camera->m_Viewport.reset();
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
		}
		
		void Update() const {
			SDL_GL_SwapWindow(m_Window.get());
		}
		
		[[nodiscard]] std::string Name() const {
			return SDL_GetWindowTitle(m_Window.get());
		}
		
		void Name(const std::string& _value) {
			SDL_SetWindowTitle(m_Window.get(), _value.c_str());
		}
		
		void Dimensions(const size_t& _width, const size_t& _height)  {
			
			SDL_SetWindowSize(m_Window.get(), static_cast<const int>(_width), static_cast<const int>(_height));
		
			SetDirty();
		}
		
		/**
		 * @brief Returns the window's x,y dimensions as a c-style array.
		 *
		 * @return glm::ivec2 The window dimensions as a glm::ivec2 object.
		 */
		[[nodiscard]] glm::vec<2, size_t> Dimensions() const override {
		
			glm::vec<2, int> size(-1, -1);
		
			SDL_GetWindowSize(m_Window.get(), &size[0], &size[1]);
		
			size.x = std::max(size.x, 0);
			size.y = std::max(size.y, 0);
			
			return static_cast<glm::vec<2, size_t>>(size);
		}
	
		[[nodiscard]] bool Focused() const {
			return GetFlag(SDL_WINDOW_INPUT_FOCUS);
		}
		
		[[nodiscard]] Uint32 GetFlags() const {
			return SDL_GetWindowFlags(m_Window.get());
		}
		
		[[nodiscard]] bool GetFlag(const Uint32& _flag) const {
			
			auto flags = GetFlags();
			
			return (flags & _flag) != 0U;
		}
		
		void SetDirty() {
			m_IsDirty = true;
		}
	
		bool IsDirty() const override {
			return m_IsDirty;
		}
		
		operator SDL_Window*() const { return m_Window.get(); }
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_WINDOW_HPP