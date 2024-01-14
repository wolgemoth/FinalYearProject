#include "Scene.h"

#include "../../../usr/scripts/Ball.h"
#include "../../../usr/scripts/FlyCam.h"
#include "../../../usr/scripts/OrbitCam.h"
#include "../../../usr/scripts/Plane.h"
#include "../../../usr/scripts/Player.h"

#include "../audio/AudioListener.h"
#include "../core/File.h"
#include "../core/Script.h"
#include "../core/Serialisation.h"
#include "../core/Transform.h"
#include "../core/utils/Hashmap.h"
#include "../ecs/Component.h"
#include "../graphics/Camera.h"
#include "../graphics/Light.h"
#include "../graphics/Renderer.h"
#include "../physics/Collider.h"
#include "../physics/Collision.h"
#include "../physics/Rigidbody.h"

#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>

#include <cstddef>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson::ECS {
	
	Scene::~Scene() {
		m_Components.Clear();
	}
	
	void Scene::Draw() {
		
		std::vector<std::shared_ptr<Component>> items;
		
		/* GET ALL RENDERERS */
		std::vector<std::shared_ptr<Graphics::Renderer>> casted_renderers;
		
		if (m_Components.Get(typeid(Graphics::Renderer), items)) {
			for (const auto& item: items) {
				casted_renderers.emplace_back(std::dynamic_pointer_cast<Graphics::Renderer>(item));
			}
		}
		
		/* GET ALL LIGHTS */
		std::vector<std::shared_ptr<Graphics::Light>> casted_lights;
		
		if (m_Components.Get(typeid(Graphics::Light), items)) {
			for (const auto& item: items) {
				casted_lights.emplace_back(std::dynamic_pointer_cast<Graphics::Light>(item));
			}
		}
		
		/* GET ALL CAMERAS */
		if (m_Components.Get(typeid(Graphics::Camera), items)) {
			
			/* RENDER */
			for (const auto& item: items) {
				
				const auto camera = std::dynamic_pointer_cast<Graphics::Camera>(item);
				camera->PreRender();
				
				camera->Clear();
				
				camera->Render(casted_renderers, casted_lights);
				camera->PostRender();
			}
		}
	}
	
	void Scene::Begin() {
	
		std::vector<std::shared_ptr<Component>> items;
		if (m_Components.Get(typeid(Script), items)) {
			for (const auto& item : items) {
				std::dynamic_pointer_cast<Script>(item)->Begin();
			}
		}
	}
	
	void Scene::Tick() {
		
		std::vector<std::shared_ptr<Component>> items;
		
		// Interpolate Rigidbodies:
		if (m_Components.Get(typeid(Physics::Rigidbody), items)) {
	
			for (const auto& item : items) {
				
				auto rigidbody = std::dynamic_pointer_cast<Physics::Rigidbody>(item);
				rigidbody->Interpolate();
			}
		}
		
		// Update AudioListeners:
		if (m_Components.Get(typeid(Audio::AudioListener), items)) {
	
			for (const auto& item : items) {
				
				auto audioListener = std::dynamic_pointer_cast<Audio::AudioListener>(item);
				audioListener->Tick();
			}
		}
		
		// Update scripts:
		if (m_Components.Get(typeid(Script), items)) {
			for (const auto& item : items) {
				std::dynamic_pointer_cast<Script>(item)->Tick();
			}
		}
	
		Draw();
	}
	
	void Scene::FixedTick() {
	
		std::vector<std::shared_ptr<Component>> items;
		
		// Update rigidbodies:
		if (m_Components.Get(typeid(Physics::Rigidbody), items)) {
	
			for (const auto& item : items) {
				
				auto rigidbody = std::dynamic_pointer_cast<Physics::Rigidbody>(item);
				rigidbody->Sync();
			}
		}
		
		// Handle physics-based operations on Scripts.
		if (m_Components.Get(typeid(Script), items)) {
	
			for (const auto& item : items) {
				
				auto script = std::dynamic_pointer_cast<Script>(item);
				
				/*
				 * Invoke collision event for every collision that the attached
				 * rigidbody component has encountered.
				 */
				{
					// Get rigidbody on Script's parent.
					const auto rb = script->Parent()->GetComponent<Physics::Rigidbody>();
					
					// Check if the pointer is valid.
					if (rb != nullptr) {
						
						// Invoke collision event for every Collision:
						const auto collisions = rb->Collisions();
						
						for (auto collision : collisions) {
							script->OnCollision(collision);
						}
					}
				}
				
				// Run the script's FixedTick().
				script->FixedTick();
			}
		}
	}
	
	const Hashmap<std::type_index, std::vector<std::shared_ptr<Component>>>& Scene::Components() noexcept {
		return m_Components;
	}
	
	void Scene::Save(const std::filesystem::path& _path) {
	
		std::cout << "Saving Scene... ";
	
		{
			auto ofStream = std::ofstream(_path);
	
			auto xml = cereal::XMLOutputArchive(ofStream);
	
			auto map = Components().GetAll();
	
			xml.setNextName("Entities"); // Start "Entities"
			xml.startNode();
	
			for (const auto& category : map) {
	
				const auto& entries = category.second;
	
				for (const auto& entry : entries) {
	
					if (category.first == typeid(GameObject)) { // Only serialise things attached to GameObject.
	
						auto entity = std::dynamic_pointer_cast<GameObject>(entry);
	
						auto subCategories = entity->Components().GetAll();
	
						xml.setNextName(entity->Name().c_str());
						xml.startNode();
	
						for (const auto& subCategory : subCategories) {
	
							const auto& components = subCategory.second;
	
							if (subCategory.first == typeid(Transform)) {		// Serialise Transform.
	
								for (const auto& component : components) {
	
									auto transform = std::dynamic_pointer_cast<Transform>(component);
	
									xml.setNextName("Transform");
									xml.startNode();
	
									xml(cereal::make_nvp("Position", Serialisation::Serialise(transform->m_Position)));
									xml(cereal::make_nvp("Rotation", Serialisation::Serialise(transform->m_Rotation)));
									xml(cereal::make_nvp("Scale",    Serialisation::Serialise(transform->m_Scale)));
	
									xml.finishNode();
								}
							}
							else if (subCategory.first == typeid(Physics::Rigidbody)) {	// Serialise Rigidbody.
	
								for (const auto& component : components) {
	
									auto rigidbody = std::dynamic_pointer_cast<Physics::Rigidbody>(component);
	
									xml.setNextName("Rigidbody");
									xml.startNode();
	
									xml(cereal::make_nvp("Velocity", Serialisation::Serialise(rigidbody->Velocity())));
									xml(cereal::make_nvp("AngularVelocity", Serialisation::Serialise(rigidbody->AngularVelocity())));
									xml(cereal::make_nvp("Mass", rigidbody->Mass()));
									xml(cereal::make_nvp("Drag", rigidbody->Drag()));
									xml(cereal::make_nvp("AngularDrag", rigidbody->AngularDrag()));
	
									xml.finishNode();
								}
							}
							else if (subCategory.first == typeid(Graphics::Camera)) {		// Serialise Camera.
	
								for (const auto& component : components) {
	
									auto camera = std::dynamic_pointer_cast<Graphics::Camera>(component);
	
									xml.setNextName("Camera");
	
									xml.startNode();
									xml.finishNode();
								}
							}
							else if (subCategory.first == typeid(Physics::Collider)) {	// Serialise Collider.
	
								for (const auto& component : components) {
	
									auto collider = std::dynamic_pointer_cast<Physics::Collider>(component);
	
									xml.setNextName("Collider");
	
									xml.startNode();
									xml(cereal::make_nvp("Type", std::to_string((int)collider->GetType())));
									xml.finishNode();
								}
							}
							else if (subCategory.first == typeid(Graphics::Light)) {    // Serialise Light.
	
								for (const auto& component : components) {
	
									auto light = std::dynamic_pointer_cast<Graphics::Light>(component);
	
									xml.setNextName("Light");
	
									xml.startNode();
									xml.finishNode();
								}
							}
							else if (subCategory.first == typeid(Graphics::Renderer)) {	// Serialise Renderer.
	
								for (const auto& component : components) {
	
									xml.setNextName(subCategory.first.name());
	
									xml.startNode();
									xml.finishNode();
								}
							}
							else if (subCategory.first == typeid(Script)) {		        // Serialise Shader.
	
								xml.setNextName("Script");
								xml.startNode();
	
								for (const auto& component : components) {
									xml(cereal::make_nvp("Type", std::string(subCategory.first.name())));
								}
	
								xml.finishNode();
							}
						}
						xml.finishNode();
					}
				}
			}
	
			xml.finishNode();
		}
	
		std::cout << "Done." << "\nOutput:\n" << File::ReadAllText(_path); // Debug output.
	}
	
	std::shared_ptr<Scene> Scene::Load(const std::filesystem::path& _path) {
	
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
			for (size_t i = 0; i < gameObjectCount; i++) {
		
				const auto *gameObjectName = xml.getNodeName();
				
				if (log) {
					std::cout << "\tName: \"" << gameObjectName << "\"\n";
				}
				
				// Create GameObject to populate.
				auto gameObject = GameObject::Create(result->shared_from_this(), gameObjectName);
		
				xml.startNode();
		
				int componentCount = 0;
				xml.loadSize(componentCount);
		
				if (log) {
					std::cout << "\tComponents: " << componentCount<< '\n';
				}
				
				for (size_t j = 0; j < componentCount; j++) {
		
					auto componentName = std::string(xml.getNodeName());
					
					if (log) {
						std::cout << "\t\t" << componentName<< '\n';
					}
					
					if (componentName == "Transform") {			// Deserialise Transform.
		
						auto transform = gameObject->AddComponent<Transform>();
		
						xml.startNode();
		
						transform->m_Position = Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, log ? 3 : -1));
						transform->m_Rotation = Serialisation::Deserialise<glm::quat>(Serialisation::ParseNext(xml, log ? 3 : -1));
						transform->m_Scale    = Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, log ? 3 : -1));
		
						xml.finishNode();
					}
					else if (componentName == "Rigidbody") {	// Deserialise Rigidbody.
		
						auto rigidbody = gameObject->AddComponent<Physics::Rigidbody>();
		
						xml.startNode();
		
						rigidbody->       Velocity(Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, log ? 3 : -1)));
						rigidbody->AngularVelocity(Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, log ? 3 : -1)));
						rigidbody->           Mass(Serialisation::Deserialise<float>    (Serialisation::ParseNext(xml, log ? 3 : -1)));
						rigidbody->           Drag(Serialisation::Deserialise<float>    (Serialisation::ParseNext(xml, log ? 3 : -1)));
						rigidbody->    AngularDrag(Serialisation::Deserialise<float>    (Serialisation::ParseNext(xml, log ? 3 : -1)));
						
						xml.finishNode();
						
						result->Attach(std::dynamic_pointer_cast<Script>(rigidbody));
					}
					else if (componentName == "Camera") {		// Deserialise Camera.
					
						auto camera = gameObject->AddComponent<Graphics::Camera>();
		
						xml.startNode();
						xml.finishNode();
					}
					else if (componentName == "Collider") {		// Deserialise Collider.
		
						auto collider = gameObject->AddComponent<Physics::Collider>();
		
						xml.startNode();
		
						std::string type;
						xml(type);
		
						collider->SetType((Physics::Collider::Type)std::stoi(type));
		
						xml.finishNode();
					}
					else if (componentName == "Light") {		// Deserialise Light
						
						auto light = gameObject->AddComponent<Graphics::Light>();
		
						xml.startNode();
						xml.finishNode();
					}
					else if (componentName == "Renderer") {		// Deserialise Renderer.
		
						auto renderer = gameObject->AddComponent<Graphics::Renderer>();
		
						xml.startNode();
						xml.finishNode();
					}
					else if (componentName == "Script") {		// Deserialise Scripts...
		
						std::shared_ptr<Script> script;
		
						xml.startNode();
		
						std::string type;
						xml(type);
		
						if (log) {
							std::cout << "\t\t\t\"" << type << "\"\n";
						}
						
						if (type == "class std::shared_ptr<class Player>") {		// Player
							script = gameObject->AddComponent<LouiEriksson::Game::Player>();
						}
						else if (type == "class std::shared_ptr<class Ball>") {		// Ball
							script = gameObject->AddComponent<LouiEriksson::Game::Ball>();
						}
						else if (type == "class std::shared_ptr<class Plane>") {	// Plane
							script = gameObject->AddComponent<LouiEriksson::Game::Plane>();
						}
						else if (type == "class std::shared_ptr<class OrbitCam>") {	// OrbitCam
							script = gameObject->AddComponent<LouiEriksson::Game::OrbitCam>();
						}
						else if (type == "class std::shared_ptr<class FlyCam>") {	// FlyCam
							script = gameObject->AddComponent<LouiEriksson::Game::FlyCam>();
						}
						else {
							std::stringstream err;
							err << "ERROR (Scene.cpp [TryLoad(std::filesystem::path)]): Deserialisation for type \"" <<
								type << "\" Has not been implemented.";
						
							throw std::runtime_error(err.str());
						}
		
						result->Attach(script);
		
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
			
			std::cout << e.what()<< '\n';
		}
		
		return result;
	}
	
} // LouiEriksson::ECS