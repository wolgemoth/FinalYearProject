#ifndef FINALYEARPROJECT_WINDOW_H
#define FINALYEARPROJECT_WINDOW_H

#include "../graphics/Camera.h"
#include "../utils/Hashmap.h"

#include "Time.h"

#include <glm/ext/vector_int2.hpp>
#include <SDL_stdinc.h>
#include <SDL_video.h>

#include <functional>
#include <memory>

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {

	class Camera;
	
	class Window {
	
		friend class Application;
		friend class Camera;
	
	private:
		
		inline static Hashmap<int, std::shared_ptr<Window>> m_Windows;
	
		int m_ID;
		SDL_GLContext m_Context;
	
		std::shared_ptr<SDL_Window> m_Window;
	
		Hashmap<int, std::reference_wrapper<Camera>> m_Cameras;
	
		void   Link(Camera& _camera);
		void Unlink(Camera& _camera);
	
		 Window(const int& _width, const int& _height, const char* _name, bool _fullscreen = false, bool _hdr10 = true);
		~Window() = default;
	
	public:
	
		Window             (const Window& _other) = delete;
		Window& operator = (const Window& _other) = delete;
	
		static std::shared_ptr<Window> Create(const int& _width, const int& _height, const char* _name);
	
		static std::shared_ptr<Window> Get(const int& _id);
	
		static void Destroy(const int& _id);
	
		[[nodiscard]] const int& ID() const;
		[[nodiscard]] SDL_GLContext Context() const;
	
		void Update() const;
	
		void Dimensions(const int& _width, const int& _height);
		[[nodiscard]] glm::ivec2 Dimensions() const;
	
		[[nodiscard]] float Aspect() const;
	
		[[nodiscard]] const bool Focused() const;
		
		[[nodiscard]] const Uint32 GetFlags() const;
		
		[[nodiscard]] const bool GetFlag(const Uint32& _flag) const;
		
		void SetDirty();
	
		explicit operator SDL_Window* () { return m_Window.get(); }
	};
}

#endif //FINALYEARPROJECT_WINDOW_H