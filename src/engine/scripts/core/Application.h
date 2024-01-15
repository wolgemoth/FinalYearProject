#ifndef FINALYEARPROJECT_APPLICATION_H
#define FINALYEARPROJECT_APPLICATION_H

#include "utils/Hashmap.h"

#include <memory>
#include <typeindex>

namespace LouiEriksson::ECS {
	
	class GameObject;
	
}; // LouiEriksson::ECS

namespace LouiEriksson {

	class Script;
	
	/// <summary> Main class for the application. </summary>
	class Application {
	
	private:
	
		inline static bool  s_Quit        { false };
		inline static bool  s_Initialised { false };
	
	public:
	
		 Application()                          = delete;
		 Application(const Application& _other) = delete;
		~Application()                          = delete;
		
		Application& operator = (const Application&  _other) = delete;
		Application& operator =       (Application&& _other) = delete;
	
		/// <summary> Main function of the application. </summary>
		static int Main(const Hashmap<std::type_index, std::shared_ptr<Script> (*)(const std::weak_ptr<ECS::GameObject>& _parent)>& _initialisers);
		
		/// <summary> Quits the application. </summary>
		static void Quit() noexcept;
	
	};
} // LouiEriksson

#endif //FINALYEARPROJECT_APPLICATION_H