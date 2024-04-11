#ifndef FINALYEARPROJECT_APPLICATION_H
#define FINALYEARPROJECT_APPLICATION_H

#include "utils/Hashmap.h"

#include <memory>
#include <string>

namespace LouiEriksson::Engine::ECS {
	
	class GameObject;
	
}; // LouiEriksson::Engine::ECS

namespace LouiEriksson::Engine {

	class Script;
	
	/// <summary> Main class for the application. </summary>
	class Application {
	
	private:
	
		inline static bool s_Quit        { false }; // Is Application scheduled to quit?
		inline static bool s_Initialised { false }; // Is Application initialised?
	
		/// <summary> Invoked when the Application is finalising. </summary>
		static void Finalise();
		
		/// <summary> Invoked when the Application unexpectedly terminates. </summary>
		static void OnTerminate();
		
	public:
	
		 Application()                          = delete;
		 Application(const Application& _other) = delete;
		~Application()                          = delete;
		
		Application& operator = (const Application&  _other) = delete;
		Application& operator =       (Application&& _other) = delete;
	
		/// <summary> Main function of the application. </summary>
		static int Main(const Hashmap<std::string, std::shared_ptr<Script> (*)(const std::weak_ptr<ECS::GameObject>& _parent)>& _values);
		
		/// <summary> Quits the application. </summary>
		static void Quit() noexcept;
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_APPLICATION_H