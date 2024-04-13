#ifndef FINALYEARPROJECT_WINDOW_H
#define FINALYEARPROJECT_WINDOW_H

#include "../core/utils/Hashmap.h"

#include <glm/ext/vector_int2.hpp>
#include <SDL_stdinc.h>
#include <SDL_video.h>

#include <functional>
#include <memory>

namespace LouiEriksson::Engine::Graphics {
	
	class Camera;
	
} // LouiEriksson::Engine::Graphics

namespace LouiEriksson::Engine {

	class Window {
	
		friend class Application;
		
		friend Graphics::Camera;
	
	private:
		
		inline static Hashmap<int, std::shared_ptr<Window>> m_Windows;
	
		int m_ID;
		SDL_GLContext m_Context;
	
		std::shared_ptr<SDL_Window> m_Window;
	
		Hashmap<int, std::reference_wrapper<Graphics::Camera>> m_Cameras;
	
		void   Link(Graphics::Camera& _camera);
		void Unlink(Graphics::Camera& _camera) noexcept;
	
		 Window(const int& _width, const int& _height, const char* _name, bool _fullscreen = false, bool _hdr10 = true);
	
	public:
	
		~Window() = default;
		
		Window             (const Window& _other) = delete;
		Window& operator = (const Window& _other) = delete;
	
		static std::shared_ptr<Window> Create(const int& _width, const int& _height, const char* _name);
	
		static std::weak_ptr<Window> Get(const int& _id);
	
		static std::weak_ptr<Window> GetFocused();
		
		static void Destroy(const int& _id);
	
		[[nodiscard]] const int& ID() const noexcept;
		[[nodiscard]] SDL_GLContext Context() const noexcept;
	
		void Update() const;
	
		void Dimensions(const int& _width, const int& _height);
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