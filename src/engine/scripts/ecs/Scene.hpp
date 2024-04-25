#ifndef FINALYEARPROJECT_SCENE_HPP
#define FINALYEARPROJECT_SCENE_HPP

#include "../core/Debug.hpp"
#include "../core/Script.hpp"
#include "../core/Serialisation.hpp"
#include "../core/Transform.hpp"
#include "../core/utils/Hashmap.hpp"
#include "../graphics/Camera.hpp"
#include "../graphics/Light.hpp"
#include "../graphics/Renderer.hpp"
#include "../physics/Collider.hpp"
#include "../physics/Rigidbody.hpp"

#include "GameObject.hpp"

#include <cereal/archives/xml.hpp>
#include <cereal/cereal.hpp>

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

namespace LouiEriksson::Engine {
	
	class Application;
	class Script;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::ECS {
	
	class Component;
	
	/**
	 * @class Scene
	 * @brief Represents a scene in the application.
	 *
	 * The Scene class is responsible for managing entities and components within a scene.
	 */
	class Scene final : public std::enable_shared_from_this<Scene> {
	
		friend LouiEriksson::Engine::Application;
		friend GameObject;
	
	private:
		
		/**
		 * @fn void Scene::Draw(const LouiEriksson::Engine::Graphics::Camera::RenderFlags& _flags)
		 * @brief Render the Scene.
		 *
		 * @param[in] _flags The render flags specifying what actions to take during the render process.
		 */
		void Draw(const LouiEriksson::Engine::Graphics::Camera::RenderFlags& _flags) {
			
			const auto size = m_Entities.size();
			
			/* GET ALL RENDERERS */
			std::vector<std::weak_ptr<Graphics::Renderer>> casted_renderers;
			
			for (size_t i = 0U; i < size; ++i) {
				
				const auto entity = m_Entities[i];
				
				if (const auto& renderers = entity->Components().Get(typeid(Graphics::Renderer))) {
					for (const auto& item : *renderers) {
						
						const auto r = std::dynamic_pointer_cast<Graphics::Renderer>(item);
						
						if (auto t = r->GetTransform().lock()) {
							t->World(); // Recalculate the transform's TRS matrix.
							
							casted_renderers.emplace_back(r);
						}
					}
				}
			}
			
			
			/* GET ALL LIGHTS */
			std::vector<std::weak_ptr<Graphics::Light>> casted_lights;
			
			for (size_t i = 0U; i < size; ++i) {
				
				const auto entity = m_Entities[i];
				
				if (const auto& lights = entity->Components().Get(typeid(Graphics::Light))) {
					for (const auto& item : *lights) {
						casted_lights.emplace_back(std::dynamic_pointer_cast<Graphics::Light>(item));
					}
				}
			}
			
			/* GET ALL CAMERAS */
			for (size_t i = 0U; i < size; ++i) {
				
				const auto entity = m_Entities[i];
				
				if (const auto& cameras = entity->Components().Get(typeid(Graphics::Camera))) {
					for (const auto& item : *cameras) {
						
						try {
							
							/* RENDER */
							const auto camera = std::dynamic_pointer_cast<Graphics::Camera>(item);
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
		
	protected:
	
		/** @brief Entities within the Scene. */
		std::vector<std::shared_ptr<GameObject>> m_Entities;
		
		/**
		 * @brief Called every frame.
		 * @param[in] _flags The render flags specifying what actions to take during the render process.
		 */
		void Tick(const Graphics::Camera::RenderFlags& _flags) {
			
			auto size = m_Entities.size();
			
			/* INTERPOLATE RIGIDBODIES */
			for (size_t i = 0U; i < size; ++i) {
				
				const auto entity = m_Entities[i];
				
				if (const auto& rigidbodies = entity->Components().Get(typeid(Physics::Rigidbody))) {
					for (const auto& item : *rigidbodies) {
						
						try {
							const auto rigidbody = std::dynamic_pointer_cast<Physics::Rigidbody>(item);
							rigidbody->Interpolate();
						}
						catch (const std::exception& e) {
							Debug::Log(e);
						}
					}
				}
			}
			
			/* UPDATE SCRIPTS */
			for (size_t i = 0U; i < size; ++i) {
				
				const auto entity = m_Entities[i];
				
				if (const auto& scripts = entity->Components().Get(typeid(Script))) {
					for (const auto& item : *scripts) {
						std::dynamic_pointer_cast<Script>(item)->Invoke();
					}
				}
			}
			
			/* UPDATE AUDIO LISTENERS */
			for (size_t i = 0U; i < size; ++i) {
				
				const auto entity = m_Entities[i];
				
				if (const auto& scripts = entity->Components().Get(typeid(Audio::AudioListener))) {
					for (const auto& item : *scripts) {
						std::dynamic_pointer_cast<Script>(item)->Invoke();
					}
				}
			}
			
			/* UPDATE AUDIO SOURCES */
			for (size_t i = 0U; i < size; ++i) {
				
				const auto entity = m_Entities[i];
				
				if (const auto& scripts = entity->Components().Get(typeid(Audio::AudioSource))) {
					for (const auto& item : *scripts) {
						std::dynamic_pointer_cast<Script>(item)->Invoke();
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
	
		/** @brief Called every physics update. */
		void FixedTick() {
		
			auto size = m_Entities.size();
			
			/* UPDATE RIGIDBODIES */
			for (size_t i = 0U; i < size; ++i) {
				
				const auto entity = m_Entities[i];
				
				if (const auto& rigidbodies = entity->Components().Get(typeid(Physics::Rigidbody))) {
					for (const auto& item : rigidbodies.value()) {
						
						try {
							auto rigidbody = std::dynamic_pointer_cast<Physics::Rigidbody>(item);
							rigidbody->Sync();
						}
						catch (const std::exception& e) {
							Debug::Log(e);
						}
					}
				}
			}
			
			/* SCRIPT FIXED-TICK */
			for (size_t i = 0U; i < size; ++i) {
				
				const auto entity = m_Entities[i];
				
				if (const auto& items = entity->Components().Get(typeid(Script))) {
					for (const auto& item : *items) {
						
						try {
							const auto script = std::dynamic_pointer_cast<Script>(item);
							
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
		
	public:
	
		~Scene() {
			m_Entities.clear();
		}
		
		/**
		 * @brief Creates a new GameObject and attaches it to the specified Scene.
		 *
		 * This function creates a new GameObject and attaches it to the specified Scene.
		 *
		 * @param[in] _scene The Scene to attach the GameObject to.
		 * @param[in] _name The optional name of the GameObject.
		 * @return A shared pointer to the newly created GameObject.
		 */
		[[nodiscard]] std::weak_ptr<GameObject> Create(const std::string_view& _name = "")  {
			return m_Entities.emplace_back(new GameObject(weak_from_this(), _name.data()), [](GameObject* _ptr) { delete _ptr; });
		}
		
		/**
		 * @fn void Scene::Save(const path &_path)
		 * @brief Save the Scene in XML format at a given path.
		 *
		 * @param[in] _path - The path to save the Scene in xml format.
		 */
		void Save(const std::filesystem::path& _path) {
		
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
						else {
							Debug::Log("Serialisation for type \"" + std::string(kvp.first.name()) + "\" has not been implemented.", LogType::Warning);
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
		}
		
		/**
		 * @brief Loads a scene from a file.
		 *
		 * @param[in] _path - The path to the scene file.
		 * @param[in] _initialisers - A Hashmap of scripts types and their initialisers.
		 * @return std::shared_ptr<Scene> - A shared pointer to the loaded scene.
		 */
		static std::shared_ptr<Scene> Load(const std::filesystem::path& _path, const Hashmap<std::string, std::shared_ptr<Script> (*)(const std::weak_ptr<ECS::GameObject>& parent)>& _initialisers) {
		
			Debug::Log("Loading Scene \"" + _path.string() + "\"... ", LogType::Info, true);
			
			auto result = std::make_shared<Scene>();
			
			// TODO: Better logging.
			const auto log = false;
			
			try {
				
				auto ifStream = std::ifstream(_path, std::ifstream::in);
			
				auto xml = cereal::XMLInputArchive(ifStream);
				xml.startNode();
			
				auto gameObjectCount = 0;
				xml.loadSize(gameObjectCount);
			
				Debug::Assert(!log, "GameObjects: " + std::to_string(gameObjectCount), LogType::Info);
				
				for (auto i = 0; i < gameObjectCount; ++i) {
			
					const char* const goName = xml.getNodeName();
					
					Debug::Assert(!log, "\tName: \"" + std::string(goName) + "\"", LogType::Info);
	
					// Create GameObject to populate.
					auto go = result->Create(goName).lock();
			
					xml.startNode();
			
					auto count = 0;
					xml.loadSize(count);
					
					Debug::Assert(!log, "\tComponents: " + std::to_string(count), LogType::Info);
					
					for (auto j = 0; j < count; j++) {
			
						const auto* const name = xml.getNodeName();
						
						Debug::Assert(!log, "\t\t" + std::string(name), LogType::Info);
						
						if (strcmp(name, typeid(Transform).name()) == 0) {			// Deserialise Transform.
			
							if (const auto t = go->AddComponent<Transform>()) {
			
								xml.startNode();
				
								t->Position(Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, log ? 3 : -1)));
								t->Rotation(Serialisation::Deserialise<glm::quat>(Serialisation::ParseNext(xml, log ? 3 : -1)));
								t->Scale   (Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, log ? 3 : -1)));
				
								xml.finishNode();
							}
						}
						else if (strcmp(name, typeid( Physics::Rigidbody).name()) == 0) {	// Deserialise Rigidbody.
			
							if (const auto r = go->AddComponent<Physics::Rigidbody>()) {
			
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
						
							if (const auto c = go->AddComponent<Graphics::Camera>()) {
								
								xml.startNode();
								xml.finishNode();
							}
						}
						else if (strcmp(name, typeid(Graphics::Light    ).name()) == 0) {		// Deserialise Light
							
							if (const auto l = go->AddComponent<Graphics::Light>()) {
			
								xml.startNode();
								xml.finishNode();
							}
						}
						else if (strcmp(name, typeid(Graphics::Renderer ).name()) == 0) {		// Deserialise Renderer.
			
							if (const auto r = go->AddComponent<Graphics::Renderer>()) {
				
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
									
									auto script = (*fPtr)(go->weak_from_this());
									go->Attach(std::move(script));
								}
								else {
									
									std::ostringstream err;
									err << "Deserialisation for type \"" << type << "\" has not been implemented.\n";
								
									throw(std::runtime_error(err.str()));
								}
								
							}
							catch (const std::exception& e) {
								Debug::Log(e);
							}
			
							xml.finishNode();
						}
						else {
							Debug::Log("Deserialisation for type \"" + std::string(name) + "\" has not been implemented.", LogType::Warning);
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
	};
	
} // LouiEriksson::Engine::ECS

#endif //FINALYEARPROJECT_SCENE_HPP