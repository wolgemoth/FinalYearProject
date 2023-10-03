#pragma once

#ifndef FINALYEARPROJECT_WINDOW_H
#define FINALYEARPROJECT_WINDOW_H

#include "Hashmap.h"
#include "Camera.h"

namespace LouiEriksson {
	
	class Window {
	
		friend class Application;
		friend class Camera;
	
	private:
		
		inline static Hashmap<size_t, std::shared_ptr<Window>> m_Windows;
	
		uint m_ID;
	
		SDL_Window* m_Window;
	
		Hashmap<size_t, std::reference_wrapper<Camera>> m_Cameras;
	
		 Window(int _width, int _height, const char* _name);
	    ~Window();
		
		void   Link(Camera& _camera);
		void Unlink(Camera& _camera);
	
	public:
		
		Window(const Window &_other) = delete;
		
		static std::shared_ptr<Window> Create(int _width, int _height, const char* _name);
	
		static std::shared_ptr<Window> Get(uint _id);
	
		static void Destroy(int _id);
	
		[[nodiscard]] uint ID() const;
	
		void Update();
	
		void Dimensions(int _width, int _height);
		int* Dimensions();
	
		float Aspect();
	
		void SetDirty();
	
		Window& operator = (const Window &_other) = delete;
		
		explicit operator SDL_Window*() { return m_Window; }
	};
}

#endif //FINALYEARPROJECT_WINDOW_H