#include "Scene.h"

#include "../audio/AudioListener.h"
#include "../core/File.h"
#include "../core/Script.h"
#include "../core/Serialisation.h"
#include "../core/Transform.h"
#include "../core/utils/Hashmap.h"
#include "../ecs/Component.h"
#include "../ecs/GameObject.h"
#include "../graphics/Camera.h"
#include "../graphics/Light.h"
#include "../graphics/Renderer.h"
#include "../physics/Collider.h"
#include "../physics/Collision.h"
#include "../physics/Rigidbody.h"

#include <cereal/archives/xml.hpp>
#include <cereal/cereal.hpp>

#include <cstddef>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace LouiEriksson::Engine::ECS {
	
	Scene::~Scene() {
		m_Components.Clear();
	}
	
	void Scene::Draw(const Graphics::Camera::RenderFlags& _flags) {
		
		std::vector<std::weak_ptr<Component>> items;
		
		/* GET ALL RENDERERS */
		std::vector<std::weak_ptr<Graphics::Renderer>> casted_renderers;
		
		if (m_Components.Get(typeid(Graphics::Renderer), items)) {
			for (const auto& item: items) {
				
				if (const auto l = item.lock()) {
					casted_renderers.emplace_back(std::dynamic_pointer_cast<Graphics::Renderer>(l));
				}
			}
		}
		
		/* GET ALL LIGHTS */
		std::vector<std::weak_ptr<Graphics::Light>> casted_lights;
		
		if (m_Components.Get(typeid(Graphics::Light), items)) {
			for (const auto& item: items) {
				
				if (const auto l = item.lock()) {
					casted_lights.emplace_back(std::dynamic_pointer_cast<Graphics::Light>(l));
				}
			}
		}
		
		/* GET ALL CAMERAS */
		if (m_Components.Get(typeid(Graphics::Camera), items)) {
			
			/* RENDER */
			for (const auto& item: items) {
				
				if (const auto l = item.lock()) {
					
					try {
						
						const auto camera = std::dynamic_pointer_cast<Graphics::Camera>(l);
						camera->PreRender(_flags);
						camera->Render(casted_renderers, casted_lights);
						camera->PostRender();
					}
					catch (const std::exception& e) {
						std::cerr << e.what() << std::endl;
					}
				}
			}
		}
	}
	
	void Scene::Begin() {
		
		std::vector<std::weak_ptr<Component>> items;
		
		if (m_Components.Get(typeid(Script), items)) {
			for (const auto& item : items) {
				
				if (const auto l = item.lock()) {
					
					try {
						std::dynamic_pointer_cast<Script>(l)->Begin();
					}
					catch (const std::exception& e) {
						std::cerr << e.what() << std::endl;
					}
				}
			}
		}
	}
	
	void Scene::Tick(const Graphics::Camera::RenderFlags& _flags) {
		
		std::vector<std::weak_ptr<Component>> items;
		
		/* INTERPOLATE RIGIDBODIES */
		if (m_Components.Get(typeid(Physics::Rigidbody), items)) {
			for (const auto& item : items) {
				
				if (const auto l = item.lock()) {
					
					try {
						const auto rigidbody = std::dynamic_pointer_cast<Physics::Rigidbody>(l);
						rigidbody->Interpolate();
					}
					catch (const std::exception& e) {
						std::cerr << e.what() << std::endl;
					}
				}
			}
		}
		
		/* UPDATE SCRIPTS */
		if (m_Components.Get(typeid(Script), items)) {
			for (const auto& item : items) {
				
				if (const auto l = item.lock()) {
					
					try {
						std::dynamic_pointer_cast<Script>(l)->Tick();
					}
					catch (const std::exception& e) {
						std::cerr << e.what() << std::endl;
					}
				}
			}
		}
		
		try {
			Draw(_flags);
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
	
	void Scene::FixedTick() {
	
		std::vector<std::weak_ptr<Component>> items;
		
		/* UPDATE RIGIDBODIES */
		if (m_Components.Get(typeid(Physics::Rigidbody), items)) {
			for (const auto& item : items) {
				
				if (const auto l = item.lock()) {
					
					try {
						auto rigidbody = std::dynamic_pointer_cast<Physics::Rigidbody>(l);
						rigidbody->Sync();
					}
					catch (const std::exception& e) {
						std::cerr << e.what() << std::endl;
					}
				}
			}
		}
		
		/* SCRIPT FIXED-TICK */
		if (m_Components.Get(typeid(Script), items)) {
			for (const auto& item : items) {
				
				if (const auto l = item.lock()) {
					
					try {
						const auto script = std::dynamic_pointer_cast<Script>(l);
						
						try {
							
							/*
							 * Invoke collision event for every collision that the attached
							 * rigidbody component has encountered.
							 */
							{
								// Get rigidbody on Script's parent.
								if (const auto  p = script->Parent().lock()) {
								if (const auto rb = p->GetComponent<Physics::Rigidbody>().lock()) {
									
									// Invoke collision event for every Collision:
									const auto collisions = rb->Collisions();
									
									for (auto collision : collisions) {
										
										try {
											script->OnCollision(collision);
										}
										catch (const std::exception& e) {
											std::cerr << e.what() << std::endl;
										}
									}
								}}
							}
						}
						catch (const std::exception& e) {
							std::cerr << e.what() << std::endl;
						}
						
						try {
							// Run the script's FixedTick().
							script->FixedTick();
						}
						catch (const std::exception& e) {
							std::cerr << e.what() << std::endl;
						}
					}
					catch (const std::exception& e) {
						std::cerr << e.what() << std::endl;
					}
				}
			}
		}
	}
	
	const Hashmap<std::type_index, std::vector<std::weak_ptr<Component>>>& Scene::Components() noexcept {
		return m_Components;
	}
	
	void Scene::Save(const std::filesystem::path& _path) {
	
		std::cout << "Saving Scene... ";
	
		try {
			auto ofStream = std::ofstream(_path);
	
			auto xml = cereal::XMLOutputArchive(ofStream);
	
			xml.setNextName("Entities"); // Start "Entities"
			xml.startNode();
	
			for (const auto& entity : m_Entities) { // Only serialise things attached to GameObject.

				xml.setNextName(entity->Name().c_str());
				xml.startNode();

				for (const auto& kvp : entity->Components().GetAll()) {

					const auto& components = kvp.second;

					if  (kvp.first == typeid(Transform)) {		// Serialise Transform.

						for (const auto& component : components) {

							auto transform = std::dynamic_pointer_cast<Transform>(component);

							xml.setNextName(kvp.first.name());
							xml.startNode();

							xml(cereal::make_nvp("Position", Serialisation::Serialise(transform->m_Position)));
							xml(cereal::make_nvp("Rotation", Serialisation::Serialise(transform->m_Rotation)));
							xml(cereal::make_nvp("Scale",    Serialisation::Serialise(transform->m_Scale)));

							xml.finishNode();
						}
					}
					else if (kvp.first == typeid( Physics::Rigidbody)) {	// Serialise Rigidbody.

						for (const auto& component : components) {

							auto rigidbody = std::dynamic_pointer_cast<Physics::Rigidbody>(component);

							xml.setNextName(kvp.first.name());
							xml.startNode();

							xml(cereal::make_nvp("Velocity",        Serialisation::Serialise(rigidbody->Velocity())));
							xml(cereal::make_nvp("AngularVelocity", Serialisation::Serialise(rigidbody->AngularVelocity())));
							xml(cereal::make_nvp("Mass",            rigidbody->Mass()));
							xml(cereal::make_nvp("Drag",            rigidbody->Drag()));
							xml(cereal::make_nvp("AngularDrag",     rigidbody->AngularDrag()));

							xml.finishNode();
						}
					}
					else if (kvp.first == typeid(Graphics::Camera   )) {		// Serialise Camera.

						for (const auto& component : components) {

							xml.setNextName(kvp.first.name());

							xml.startNode();
							xml.finishNode();
						}
					}
					else if (kvp.first == typeid(Graphics::Light    )) {    // Serialise Light.

						for (const auto& component : components) {

							xml.setNextName(kvp.first.name());

							xml.startNode();
							xml.finishNode();
						}
					}
					else if (kvp.first == typeid(Graphics::Renderer )) {	// Serialise Renderer.

						for (const auto& component : components) {

							xml.setNextName(kvp.first.name());

							xml.startNode();
							xml.finishNode();
						}
					}
					else if (kvp.first == typeid(Script)) {		        // Serialise Shader.

						xml.setNextName(kvp.first.name());
						xml.startNode();

						for (const auto& component : components) {
							xml(cereal::make_nvp("TypeID", std::string(component->TypeID().name())));
						}

						xml.finishNode();
					}
				}
				
				xml.finishNode();
			}
			
			xml.finishNode();
			
			std::cout << "Done.\n";
		}
		catch (const std::exception& e) {
			std::cout << "Failed.\n"; // Debug output.
			
			std::cerr << e.what() << std::endl;
		}
	
		std::cout << "Output:\n" << File::ReadAllText(_path); // Debug output.
	}
	
	std::shared_ptr<Scene> Scene::Load(const std::filesystem::path& _path, const Hashmap<std::string, std::shared_ptr<Script> (*)(const std::weak_ptr<ECS::GameObject>& _parent)>& _initialisers) {
	
		std::cout << "Loading Scene \"" << _path.c_str() << "\"... ";
		
		auto result = std::make_shared<Scene>();
		
		// TODO: Better logging.
		const auto log = false;
		
		try {
			
			auto ifStream = std::ifstream(_path, std::ifstream::in);
		
			auto xml = cereal::XMLInputArchive(ifStream);
		
			xml.startNode();
		
			int gameObjectCount = 0;
			xml.loadSize(gameObjectCount);
		
			if (log) {
				std::cout << "GameObjects: " << gameObjectCount<< '\n';
			}
			for (size_t i = 0; i < gameObjectCount; ++i) {
		
				const auto *goName = xml.getNodeName();
				
				if (log) {
					std::cout << "\tName: \"" << goName << "\"\n";
				}
				
				// Create GameObject to populate.
				auto go = GameObject::Create(result->shared_from_this(), goName);
		
				xml.startNode();
		
				int count = 0;
				xml.loadSize(count);
		
				if (log) {
					std::cout << "\tComponents: " << count<< '\n';
				}
				
				for (size_t j = 0; j < count; j++) {
		
					const auto* name = xml.getNodeName();
					
					if (log) {
						std::cout << "\t\t" << name<< '\n';
					}
					
					if (strcmp(name, typeid(Transform).name()) == 0) {			// Deserialise Transform.
		
						if (const auto t = go->AddComponent<Transform>().lock()) {
		
							xml.startNode();
			
							t->m_Position = Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, log ? 3 : -1));
							t->m_Rotation = Serialisation::Deserialise<glm::quat>(Serialisation::ParseNext(xml, log ? 3 : -1));
							t->m_Scale    = Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, log ? 3 : -1));
			
							xml.finishNode();
						}
					}
					else if (strcmp(name, typeid( Physics::Rigidbody).name()) == 0) {	// Deserialise Rigidbody.
		
						if (const auto r = go->AddComponent<Physics::Rigidbody>().lock()) {
		
							xml.startNode();
			
							r->       Velocity(Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, log ? 3 : -1)));
							r->AngularVelocity(Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, log ? 3 : -1)));
							r->           Mass(Serialisation::Deserialise<float>    (Serialisation::ParseNext(xml, log ? 3 : -1)));
							r->           Drag(Serialisation::Deserialise<float>    (Serialisation::ParseNext(xml, log ? 3 : -1)));
							r->    AngularDrag(Serialisation::Deserialise<float>    (Serialisation::ParseNext(xml, log ? 3 : -1)));
							
							xml.finishNode();
						}
					}
					else if (strcmp(name, typeid(Graphics::Camera   ).name()) == 0) {		// Deserialise Camera.
					
						if (const auto c = go->AddComponent<Graphics::Camera>().lock()) {
							
							xml.startNode();
							xml.finishNode();
						}
					}
					else if (strcmp(name, typeid(Graphics::Light    ).name()) == 0) {		// Deserialise Light
						
						if (const auto l = go->AddComponent<Graphics::Light>().lock()) {
		
							xml.startNode();
							xml.finishNode();
						}
					}
					else if (strcmp(name, typeid(Graphics::Renderer ).name()) == 0) {		// Deserialise Renderer.
		
						if (const auto r = go->AddComponent<Graphics::Renderer>().lock()) {
			
							xml.startNode();
							xml.finishNode();
						}
					}
					else if (strcmp(name, typeid(Script).name()) == 0) {		// Deserialise Scripts...
		
						xml.startNode();
		
						std::string type;
						xml(type);
		
						if (log) {
							std::cout << "\t\t\t\"" << type << "\"\n";
						}
						
						try {
						
							using FuncPtrType = std::shared_ptr<Script> (*)(const std::weak_ptr<ECS::GameObject>&);
							
							FuncPtrType fPtr;
							if (_initialisers.Get(type, fPtr)) {
								
								const auto script = fPtr(go);
								go->Attach(script->TypeID(), script);
							}
							else {
								
								std::stringstream err;
								err << "Deserialisation for type \"" << type << "\" Has not been implemented.\n";
							
								throw(std::runtime_error(err.str()));
							}
							
						}
						catch (const std::exception& e) {
							std::cerr << e.what() << std::endl;
						}
		
						xml.finishNode();
					}
				}
		
				xml.finishNode();
			}
		
			xml.finishNode();
			
			std::cout << "Done."<< '\n';
		}
		catch (const std::exception& e) {
			
			std::cout << "Failed.\n";
			
			std::cerr << e.what() << std::endl;
		}
		
		return result;
	}
	
} // LouiEriksson::Engine::ECS