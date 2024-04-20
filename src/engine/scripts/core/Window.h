#ifndef FINALYEARPROJECT_WINDOW_H
#define FINALYEARPROJECT_WINDOW_H

#include "../core/utils/Hashmap.h"

#include <glm/ext/vector_int2.hpp>
#include <SDL_stdinc.h>
#include <SDL_video.h>

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
	
		// TODO: Fix this. This is stupid.
		Hashmap<size_t, std::reference_wrapper<Graphics::Camera>> m_Cameras;
	
		void   Link(Graphics::Camera& _camera);
		void Unlink(Graphics::Camera& _camera) noexcept;
	
		 Window(const int& _width, const int& _height, const char* _name, bool _fullscreen = false, bool _hdr10 = true);
	
	public:
	
		~Window() = default;
		
		Window             (const Window& _other) = delete;
		Window& operator = (const Window& _other) = delete;
	
		static std::shared_ptr<Window> Create(const int& _width, const int& _height, const char* _name);
	
		static std::weak_ptr<Window> Get(const size_t& _id);
	
		static std::weak_ptr<Window> GetFocused();
		
		static void Destroy(const size_t& _id);
	
		[[nodiscard]] const size_t& ID() const noexcept;
		[[nodiscard]] SDL_GLContext Context() const noexcept;
		
		void Update() const;
		
		void Dimensions(const int& _width, const int& _height);
		
		/**
		 * @brief Returns the window's x,y dimensions as a c-style array.
		 *
		 * @return glm::ivec2 The window dimensions as a glm::ivec2 object.
		 */
		[[nodiscard]] glm::ivec2 Dimensions() const;
	
		[[nodiscard]] float Aspect() const;
	
		[[nodiscard]] bool Focused() const;
		
		[[nodiscard]] Uint32 GetFlags() const;
		
		[[nodiscard]] bool GetFlag(const Uint32& _flag) const;
		
		void SetDirty();
	
		explicit operator SDL_Window* () { return m_Window.get(); }
		
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_WINDOW_H