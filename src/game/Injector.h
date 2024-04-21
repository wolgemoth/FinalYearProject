#ifndef GAME_SCRIPTS_H
#define GAME_SCRIPTS_H

#include "../engine/scripts/core/Script.h"
#include "../engine/scripts/core/utils/Hashmap.h"
#include "../engine/scripts/ecs/GameObject.h"
#include "../engine/scripts/core/Debug.h"

#include "scripts/Ball.h"
#include "scripts/FlyCam.h"
#include "scripts/Plane.h"
#include "scripts/Spatial/Map.h"
#include "scripts/Spatial/Planetarium.h"
#include "scripts/Spatial/Stars.h"

#include <exception>
#include <memory>
#include <string>

using namespace LouiEriksson::Engine;
using namespace LouiEriksson::Game::Scripts;

namespace LouiEriksson::Game::Core {
	
	/**
	 * @class Injector
	 *
	 * @brief The Injector class provides a mechanism for registering and retrieving function pointers
	 * for custom script initialisation.
	 */
	class Injector final {
	
	private:
		
		/** @brief Hashmap of function pointers for custom script initialisation indexed by the name of their type. */
		inline static Hashmap<std::string, std::shared_ptr<Script> (*)(const std::weak_ptr<ECS::GameObject>& parent)> s_Initialisers;
		
	public:
		
		/**
		 * @brief Returns function pointers for the initialisation of custom Scripts loosely-coupled from the engine runtime.
		 *
		 * @details This function checks if the static Hashmap of initialisers (s_Initialisers) is empty.
		 * If it is, it populates it with function pointers for the initialisation of various user scripts,
		 * each associated with the corresponding typeid name.
		 *
		 * If an exception is thrown during initialisation, it is caught and logged using Debug::Log() without interrupting execution.
		 *
		 * @return A Hashmap<std::string, std::shared_ptr<Script>(*)(const std::weak_ptr<ECS::GameObject>&)> which contains
		 * the typeid name of the game object as the key and corresponding function pointer for their initialisation as the value.
		 */
		static const Hashmap<std::string, std::shared_ptr<Script> (*)(const std::weak_ptr<ECS::GameObject>& parent)>& GetInitialisers() noexcept {
			
			try {
				
				if (s_Initialisers.empty()) {
					
					/*
					 * Create function pointers for the initialisation of custom Scripts
					 * loosely-coupled from the engine runtime.
					 *
					 * TODO: Automatically generate this in the future at build time:
					 */
					
					s_Initialisers.Add(
						typeid(std::shared_ptr<Ball>).name(),
						[](const std::weak_ptr<ECS::GameObject>& _parent) {
							return std::shared_ptr<Script>(new Ball(_parent));
						}
					);
					
					s_Initialisers.Add(
						typeid(std::shared_ptr<FlyCam>).name(),
						[](const std::weak_ptr<ECS::GameObject>& _parent) {
							return std::shared_ptr<Script>(new FlyCam(_parent));
						}
					);
					
					s_Initialisers.Add(
						typeid(std::shared_ptr<Plane>).name(),
						[](const std::weak_ptr<ECS::GameObject>& _parent) {
							return std::shared_ptr<Script>(new Plane(_parent));
						}
					);
					
					s_Initialisers.Add(
						typeid(std::shared_ptr<Scripts::Spatial::Map>).name(),
						[](const std::weak_ptr<ECS::GameObject>& _parent) {
							return std::shared_ptr<Script>(new Scripts::Spatial::Map(_parent));
						}
					);
					
					s_Initialisers.Add(
						typeid(std::shared_ptr<Scripts::Spatial::Planetarium>).name(),
						[](const std::weak_ptr<ECS::GameObject>& _parent) {
							return std::shared_ptr<Script>(new Scripts::Spatial::Planetarium(_parent));
						}
					);
					
					s_Initialisers.Add(
						typeid(std::shared_ptr<Scripts::Spatial::Stars>).name(),
						[](const std::weak_ptr<ECS::GameObject>& _parent) {
							return std::shared_ptr<Script>(new Scripts::Spatial::Stars(_parent));
						}
					);
				}
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
			
			return s_Initialisers;
		}
		
	};

} // LouiEriksson::Game::Core

#endif //GAME_SCRIPTS_H