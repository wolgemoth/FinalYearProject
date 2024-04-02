#ifndef GAME_SCRIPTS_H
#define GAME_SCRIPTS_H

#include "../engine/scripts/core/Script.h"
#include "../engine/scripts/core/utils/Hashmap.h"
#include "../engine/scripts/ecs/GameObject.h"

#include "scripts/Ball.h"
#include "scripts/FlyCam.h"
#include "scripts/OrbitCam.h"
#include "scripts/Plane.h"
#include "scripts/Planetarium.h"

#include <exception>
#include <iostream>
#include <memory>
#include <string>

using namespace LouiEriksson::Engine;
using namespace LouiEriksson::Game::Scripts;

namespace LouiEriksson::Game::Core {

	/// <summary> Generates the function pointers necessary for injecting custom scripts into the engine. </summary>
	class Injector {
	
	private:
		
		/// <summary> Hashmap of function pointers for custom script initialisation indexed by the name of their type. <summary>
		inline static Hashmap<std::string, std::shared_ptr<Script> (*)(const std::weak_ptr<ECS::GameObject>& parent)> s_Initialisers;
		
	public:
		
		/// <summary> Get the function pointers for initialisation of custom user scripts. </summary>
		static Hashmap<std::string, std::shared_ptr<Script> (*)(const std::weak_ptr<ECS::GameObject>& parent)> GetInitialisers() noexcept {
			
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
						typeid(std::shared_ptr<OrbitCam>).name(),
						[](const std::weak_ptr<ECS::GameObject>& _parent) {
							return std::shared_ptr<Script>(new OrbitCam(_parent));
						}
					);
					
					s_Initialisers.Add(
						typeid(std::shared_ptr<Plane>).name(),
						[](const std::weak_ptr<ECS::GameObject>& _parent) {
							return std::shared_ptr<Script>(new Plane(_parent));
						}
					);
					
					s_Initialisers.Add(
						typeid(std::shared_ptr<Planetarium>).name(),
						[](const std::weak_ptr<ECS::GameObject>& _parent) {
							return std::shared_ptr<Script>(new Planetarium(_parent));
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