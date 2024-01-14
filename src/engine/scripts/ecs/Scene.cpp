#include "Scene.h"

#include "../../../usr/scripts/Ball.h"
#include "../../../usr/scripts/FlyCam.h"
#include "../../../usr/scripts/OrbitCam.h"
#include "../../../usr/scripts/Plane.h"

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
					
					const auto camera = std::dynamic_pointer_cast<Graphics::Camera>(l);
					camera->PreRender(_flags);
					camera->Render(casted_renderers, casted_lights);
					camera->PostRender();
				}
			}
		}
	}
	
	void Scene::Begin() {
	
		std::vector<std::weak_ptr<Component>> items;
		
		if (m_Components.Get(typeid(Script), items)) {
			for (const auto& item : items) {
				
				if (const auto l = item.lock()) {
					std::dynamic_pointer_cast<Script>(l)->Begin();
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
					
					const auto rigidbody = std::dynamic_pointer_cast<Physics::Rigidbody>(l);
					rigidbody->Interpolate();
				}
			}
		}
		
		/* UPDATE AUDIOLISTENERS */
		if (m_Components.Get(typeid(Audio::AudioListener), items)) {
			for (const auto& item : items) {
				
				if (const auto l = item.lock()) {
					
					const auto audioListener = std::dynamic_pointer_cast<Audio::AudioListener>(l);
					audioListener->Tick();
				}
			}
		}
		
		/* UPDATE SCRIPTS */
		if (m_Components.Get(typeid(Script), items)) {
			for (const auto& item : items) {
				
				if (const auto l = item.lock()) {
					std::dynamic_pointer_cast<Script>(l)->Tick();
				}
			}
		}
		
		Draw(_flags);
	}
	
	void Scene::FixedTick() {
	
		std::vector<std::weak_ptr<Component>> items;
		
		/* UPDATE RIGIDBODIES */
		if (m_Components.Get(typeid(Physics::Rigidbody), items)) {
			for (const auto& item : items) {
				
				if (const auto l = item.lock()) {
					
					auto rigidbody = std::dynamic_pointer_cast<Physics::Rigidbody>(l);
					rigidbody->Sync();
				}
			}
		}
		
		/* SCRIPT FIXED-TICK */
		if (m_Components.Get(typeid(Script), items)) {
			for (const auto& item : items) {
				
				if (const auto l = item.lock()) {
					
					const auto script = std::dynamic_pointer_cast<Script>(l);
					
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
								script->OnCollision(collision);
							}
						}}
					}
					
					// Run the script's FixedTick().
					script->FixedTick();
				}
			}
		}
	}
	
	const Hashmap<std::type_index, std::vector<std::weak_ptr<Component>>>& Scene::Components() noexcept {
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
	
				const auto& items = category.second;
	
				for (const auto& item : items) {
	
					if (const auto l = item.lock()) {
						
						if (category.first == typeid(GameObject)) { // Only serialise things attached to GameObject.
		
							auto entity = std::dynamic_pointer_cast<GameObject>(l);
		
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
		
					auto name = std::string(xml.getNodeName());
					
					if (log) {
						std::cout << "\t\t" << name<< '\n';
					}
					
					if (name == "Transform") {			// Deserialise Transform.
		
						if (const auto t = go->AddComponent<Transform>().lock()) {
		
							xml.startNode();
			
							t->m_Position = Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, log ? 3 : -1));
							t->m_Rotation = Serialisation::Deserialise<glm::quat>(Serialisation::ParseNext(xml, log ? 3 : -1));
							t->m_Scale    = Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, log ? 3 : -1));
			
							xml.finishNode();
						}
					}
					else if (name == "Rigidbody") {	// Deserialise Rigidbody.
		
						if (const auto r = go->AddComponent<Physics::Rigidbody>().lock()) {
		
							xml.startNode();
			
							r->       Velocity(Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, log ? 3 : -1)));
							r->AngularVelocity(Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, log ? 3 : -1)));
							r->           Mass(Serialisation::Deserialise<float>    (Serialisation::ParseNext(xml, log ? 3 : -1)));
							r->           Drag(Serialisation::Deserialise<float>    (Serialisation::ParseNext(xml, log ? 3 : -1)));
							r->    AngularDrag(Serialisation::Deserialise<float>    (Serialisation::ParseNext(xml, log ? 3 : -1)));
							
							xml.finishNode();
							
							result->Attach(std::dynamic_pointer_cast<Script>(r));
						}
					}
					else if (name == "Camera") {		// Deserialise Camera.
					
						if (const auto c = go->AddComponent<Graphics::Camera>().lock()) {
							
							xml.startNode();
							xml.finishNode();
						}
					}
					else if (name == "Collider") {		// Deserialise Collider.
		
						if (const auto c = go->AddComponent<Physics::Collider>().lock()) {
			
							xml.startNode();
			
							std::string type;
							xml(type);
			
							c->SetType((Physics::Collider::Type)std::stoi(type));
			
							xml.finishNode();
						}
					}
					else if (name == "Light") {		// Deserialise Light
						
						if (const auto l = go->AddComponent<Graphics::Light>().lock()) {
		
							xml.startNode();
							xml.finishNode();
						}
					}
					else if (name == "Renderer") {		// Deserialise Renderer.
		
						if (const auto r = go->AddComponent<Graphics::Renderer>().lock()) {
			
							xml.startNode();
							xml.finishNode();
						}
					}
					else if (name == "Script") {		// Deserialise Scripts...
		
						std::weak_ptr<Script> script;
		
						xml.startNode();
		
						std::string type;
						xml(type);
		
						if (log) {
							std::cout << "\t\t\t\"" << type << "\"\n";
						}
						
						     if (type == "class std::shared_ptr<class Ball>") {		// Ball
							script = go->AddComponent<LouiEriksson::Game::Ball>();
						}
						else if (type == "class std::shared_ptr<class Plane>") {	// Plane
							script = go->AddComponent<LouiEriksson::Game::Plane>();
						}
						else if (type == "class std::shared_ptr<class OrbitCam>") {	// OrbitCam
							script = go->AddComponent<LouiEriksson::Game::OrbitCam>();
						}
						else if (type == "class std::shared_ptr<class FlyCam>") {	// FlyCam
							script = go->AddComponent<LouiEriksson::Game::FlyCam>();
						}
						else {
							std::stringstream err;
							err << "ERROR (Scene.cpp [TryLoad(std::filesystem::path)]): Deserialisation for type \"" <<
								type << "\" Has not been implemented.";
						
							throw std::runtime_error(err.str());
						}
		
						result->Attach(script.lock());
		
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