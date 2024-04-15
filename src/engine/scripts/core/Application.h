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
	
	/**
	 * @class Application
	 * @brief Represents the application.
	 *
	 * The Application class is responsible for managing the main execution flow of the program and handling the termination of the application.
	 */
	class Application final {
	
	private:
	
		inline static bool s_Quit        { false }; // Is Application scheduled to quit?
		inline static bool s_Initialised { false }; // Is Application initialised?
		
		/**
		 * @brief Finalises the application.
		 *
		 * Releases allocated resources.
		 *
		 * @see Debug::Log(std::string_view, LogType, bool)
		 * @see Input::Cursor::Reset()
		 * @see UI::GUI::Dispose()
		 * @see Physics::Physics::Dispose()
		 * @see Input::Input::Dispose()
		 * @see Audio::Sound::Dispose()
		 * @see Networking::Requests::Dispose()
		 * @see SDL_Quit()
		 * @see Debug::Flush()
		 *
		 * @note This function should only be called when the application is about to terminate.
		 */
		static void Finalise();
		
		/**
		 * @brief Called when the application unexpectedly terminates.
		 *
		 * Perform finalisation and then exit the application.
		 *
		 * @see Debug::Log(const std::string_view&, const LogType&, const bool&)
		 * @see Utils::ALDumpError(const bool&)
		 * @see Utils::GLDumpError(const bool&)
		 * @see Application::Finalise()
		 * @see std::exit()
		 */
		static void OnTerminate();
		
	public:
	
		 Application()                          = delete;
		 Application(const Application& _other) = delete;
		~Application()                          = delete;
		
		Application& operator = (const Application&  _other) = delete;
		Application& operator =       (Application&& _other) = delete;
		
		/**
		 * @brief Entry point of the application.
		 *
		 * This function is the main entry point of the application, and contains the main loop.
		 *
		 * @param[in] _values A hashmap containing function pointers to initialise different user scripts
		 * @return An integer error code (0 for successful execution)
		 */
		static int Main(const Hashmap<std::string, std::shared_ptr<Script> (*)(const std::weak_ptr<ECS::GameObject>& _parent)>& _values);
		
		/**
		 * @brief Quit function.
		 *
		 * This function is responsible for quitting the application.
		 *
		 * @see Application::s_Quit
		 * @see Debug::Log(const std::string_view&, const LogType&, const bool&)
		 */
		static void Quit() noexcept;
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_APPLICATION_H