#include "Scene.h"

#include "../audio/AudioListener.h"
#include "../core/Debug.h"
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
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace LouiEriksson::Engine::ECS {
	
	Scene::~Scene() {
		m_Components.Clear();
	}
	
	void Scene::Draw(const Graphics::Camera::RenderFlags& _flags) {
		
		/* GET ALL RENDERERS */
		std::vector<std::weak_ptr<Graphics::Renderer>> casted_renderers;
		
		if (const auto items = m_Components.Get(typeid(Graphics::Renderer))) {
			for (const auto& item : *items) {
				
				if (const auto l = item.lock()) {
					casted_renderers.emplace_back(std::dynamic_pointer_cast<Graphics::Renderer>(l));
				}
			}
		}
		
		/* GET ALL LIGHTS */
		std::vector<std::weak_ptr<Graphics::Light>> casted_lights;
		
		if (const auto items = m_Components.Get(typeid(Graphics::Light))) {
			for (const auto& item : *items) {
				
				if (const auto l = item.lock()) {
					casted_lights.emplace_back(std::dynamic_pointer_cast<Graphics::Light>(l));
				}
			}
		}
		
		/* GET ALL CAMERAS */
		if (const auto items = m_Components.Get(typeid(Graphics::Camera))) {
			
			/* RENDER */
			for (const auto& item : *items) {
				
				if (const auto l = item.lock()) {
					
					try {
						
						const auto camera = std::dynamic_pointer_cast<Graphics::Camera>(l);
						camera->PreRender(_flags);
						camera->Render(casted_renderers, casted_lights);
						camera->PostRender();
					}
					catch (const std::exception& e) {
						Debug::Log(e);
					}
				}
			}
		}
	}
	
	void Scene::Begin() {
		
		if (const auto components = m_Components.Get(typeid(Script))) {
			
			const auto items = *components;
			
			for (const auto& item : items) {
				
				if (const auto l = item.lock()) {
					
					try {
						std::dynamic_pointer_cast<Script>(l)->Begin();
					}
					catch (const std::exception& e) {
						Debug::Log(e);
					}
				}
			}
		}
	}
	
	void Scene::Tick(const Graphics::Camera::RenderFlags& _flags) {
		
		/* INTERPOLATE RIGIDBODIES */
		if (const auto items = m_Components.Get(typeid(Physics::Rigidbody))) {
			for (const auto& item : *items) {
				
				if (const auto l = item.lock()) {
					
					try {
						const auto rigidbody = std::dynamic_pointer_cast<Physics::Rigidbody>(l);
						rigidbody->Interpolate();
					}
					catch (const std::exception& e) {
						Debug::Log(e);
					}
				}
			}
		}
		
		/* UPDATE SCRIPTS */
		if (const auto items = m_Components.Get(typeid(Script))) {
			for (const auto& item : *items) {
				
				if (const auto l = item.lock()) {
					
					try {
						std::dynamic_pointer_cast<Script>(l)->Tick();
					}
					catch (const std::exception& e) {
						Debug::Log(e);
					}
				}
			}
		}
		
		try {
			Draw(_flags);
		}
		catch (const std::exception& e) {
			Debug::Log(e);
		}
	}
	
	void Scene::FixedTick() {
	
		/* UPDATE RIGIDBODIES */
		if (const auto items = m_Components.Get(typeid(Physics::Rigidbody))) {
			for (const auto& item : items.value()) {
				
				if (const auto l = item.lock()) {
					
					try {
						auto rigidbody = std::dynamic_pointer_cast<Physics::Rigidbody>(l);
						rigidbody->Sync();
					}
					catch (const std::exception& e) {
						Debug::Log(e);
					}
				}
			}
		}
		
		/* SCRIPT FIXED-TICK */
		if (const auto items = m_Components.Get(typeid(Script))) {
			for (const auto& item : *items) {
				
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
											Debug::Log(e);
										}
									}
								}}
							}
						}
						catch (const std::exception& e) {
							Debug::Log(e);
						}
						
						try {
							// Run the script's FixedTick().
							script->FixedTick();
						}
						catch (const std::exception& e) {
							Debug::Log(e);
						}
					}
					catch (const std::exception& e) {
						Debug::Log(e);
					}
				}
			}
		}
	}
	
	const Hashmap<std::type_index, std::vector<std::weak_ptr<Component>>>& Scene::Components() noexcept {
		return m_Components;
	}
	
	void Scene::Save(const std::filesystem::path& _path) {
	
		Debug::Log("Saving Scene... ", LogType::Info, true);
		
		try {
			auto ofStream = std::ofstream(_path);
	
			auto xml = cereal::XMLOutputArchive(ofStream);
	
			xml.setNextName("Entities"); // Start "Entities"
			xml.startNode();
	
			for (const auto& entity : m_Entities) { // Only serialise things attached to GameObject.

				xml.setNextName(std::string(entity->Name()).c_str());
				xml.startNode();
				
				for (const auto& kvp : entity->Components()) {

					const auto& components = kvp.second;

					if  (kvp.first == typeid(Transform)) {		// Serialise Transform.

						for (const auto& component : components) {

							auto transform = std::dynamic_pointer_cast<Transform>(component);

							xml.setNextName(kvp.first.name());
							xml.startNode();

							xml(cereal::make_nvp("Position", Serialisation::Serialise(transform->Position())));
							xml(cereal::make_nvp("Rotation", Serialisation::Serialise(transform->Rotation())));
							xml(cereal::make_nvp("Scale",    Serialisation::Serialise(transform->Scale())));

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
			
			Debug::Log("Done.", LogType::Info);
		}
		catch (const std::exception& e) {
			Debug::Log("Failed.", LogType::Error);
			Debug::Log(e);
		}
	
		Debug::Log("Output: " + File::ReadAllText(_path), LogType::Info);
	}
	
	std::shared_ptr<Scene> Scene::Load(const std::filesystem::path& _path, const Hashmap<std::string, std::shared_ptr<Script> (*)(const std::weak_ptr<ECS::GameObject>& _parent)>& _initialisers) {
	
		Debug::Log("Loading Scene \"" + _path.string() + "\"... ", LogType::Info, true);
		
		auto result = std::make_shared<Scene>();
		
		// TODO: Better logging.
		const auto log = false;
		
		try {
			
			auto ifStream = std::ifstream(_path, std::ifstream::in);
		
			auto xml = cereal::XMLInputArchive(ifStream);
			xml.startNode();
		
			int gameObjectCount = 0;
			xml.loadSize(gameObjectCount);
		
			Debug::Assert(!log, "GameObjects: " + std::to_string(gameObjectCount), LogType::Info);
			
			for (size_t i = 0; i < gameObjectCount; ++i) {
		
				const auto *goName = xml.getNodeName();
				
				Debug::Assert(!log, "\tName: \"" + std::string(goName) + "\"", LogType::Info);

				// Create GameObject to populate.
				auto go = GameObject::Create(result->shared_from_this(), goName);
		
				xml.startNode();
		
				int count = 0;
				xml.loadSize(count);
				
				Debug::Assert(!log, "\tComponents: " + std::to_string(count), LogType::Info);
				
				for (size_t j = 0; j < count; j++) {
		
					const auto* const name = xml.getNodeName();
					
					Debug::Assert(!log, "\t\t" + std::string(name), LogType::Info);
					
					if (strcmp(name, typeid(Transform).name()) == 0) {			// Deserialise Transform.
		
						if (const auto t = go->AddComponent<Transform>().lock()) {
		
							xml.startNode();
			
							t->Position(Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, log ? 3 : -1)));
							t->Rotation(Serialisation::Deserialise<glm::quat>(Serialisation::ParseNext(xml, log ? 3 : -1)));
							t->Scale   (Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, log ? 3 : -1)));
			
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
						
						Debug::Assert(!log, "\t\t\t\"" + std::string(type) + "\"", LogType::Info);
						
						try {
						
							if (const auto fPtr = _initialisers.Get(type)) {
								
								auto script = (*fPtr)(go);
								go->Attach(std::move(script));
							}
							else {
								
								std::stringstream err;
								err << "Deserialisation for type \"" << type << "\" Has not been implemented.\n";
							
								throw(std::runtime_error(err.str()));
							}
							
						}
						catch (const std::exception& e) {
							Debug::Log(e);
						}
		
						xml.finishNode();
					}
				}
		
				xml.finishNode();
			}
		
			xml.finishNode();
			
			Debug::Log("Done.", LogType::Info);
		}
		catch (const std::exception& e) {
			Debug::Log("Failed.", LogType::Error);
			Debug::Log(e);
		}
		
		return result;
	}
	
} // LouiEriksson::Engine::ECS