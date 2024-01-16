#ifndef GAME_SCRIPTS_H
#define GAME_SCRIPTS_H

#include "../../src/engine/scripts/core/Script.h"
#include "../../src/engine/scripts/core/utils/Hashmap.h"

#include "../scripts/Ball.h"
#include "../scripts/FlyCam.h"
#include "../scripts/OrbitCam.h"
#include "../scripts/Plane.h"

#include <exception>
#include <iostream>
#include <memory>
#include <typeindex>

using namespace LouiEriksson::Engine;
using namespace LouiEriksson::Game::Scripts;

namespace LouiEriksson::Game::Core {

	/// <summary> Generates the function pointers necessary for injecting custom scripts into the engine. </summary>
	class Injector {
	
	private:
		
		/// <summary> Hashmap of function pointers for custom script initialisation indexed by type. <summary>
		inline static Hashmap<std::type_index, std::shared_ptr<Script> (*)(const std::weak_ptr<ECS::GameObject>& _parent)> s_Initialisers;
		
	public:
		
		/// <summary> Get the function pointers for initialisation of custom user scripts. </summary>
		static Hashmap<std::type_index, std::shared_ptr<Script> (*)(const std::weak_ptr<ECS::GameObject>& _parent)> GetInitialisers() noexcept {
			
			try {
				
				if (s_Initialisers.empty()) {
					
					/*
					 * Create function pointers for the initialisation of custom Scripts
					 * loosely-coupled from the engine runtime.
					 *
					 * TODO: Automatically generate this in the future at build time:
					 */
					
					s_Initialisers.Add(
						typeid(std::shared_ptr<Ball>),
						[](const std::weak_ptr<ECS::GameObject>& _parent) {
							return std::shared_ptr<Script>(new Ball(_parent));
						}
					);
					
					s_Initialisers.Add(
						typeid(std::shared_ptr<FlyCam>),
						[](const std::weak_ptr<ECS::GameObject>& _parent) {
							return std::shared_ptr<Script>(new FlyCam(_parent));
						}
					);
					
					s_Initialisers.Add(
						typeid(std::shared_ptr<OrbitCam>),
						[](const std::weak_ptr<ECS::GameObject>& _parent) {
							return std::shared_ptr<Script>(new OrbitCam(_parent));
						}
					);
					
					s_Initialisers.Add(
						typeid(std::shared_ptr<Plane>),
						[](const std::weak_ptr<ECS::GameObject>& _parent) {
							return std::shared_ptr<Script>(new Plane(_parent));
						}
					);
					
				}
			}
			catch (const std::exception& e) {
				std::cout << e.what() << '\n';
			}
			
			return s_Initialisers;
		}
		
	};

} // LouiEriksson::Game::Core

#endif //GAME_SCRIPTS_H