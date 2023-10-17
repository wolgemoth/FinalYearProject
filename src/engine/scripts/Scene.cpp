#include "stdafx.h"

#include "Scene.h"

namespace LouiEriksson {
	
	Scene::~Scene() {
		m_Entities.Clear();
	}
	
	void Scene::Draw() {
		
		std::vector<std::shared_ptr<Renderer>> casted_renderers;
		
		std::vector<std::any> renderers;
		if (m_Entities.Get(typeid(Renderer), renderers)) {
			for (auto& r: renderers) {
				casted_renderers.push_back(std::any_cast<std::shared_ptr<Renderer>>(r));
			}
		}
		
		std::vector<std::any> cameras;
		if (m_Entities.Get(typeid(Camera), cameras)) {
			
			/* RENDER */
			for (auto& c: cameras) {
				
				const auto camera = std::any_cast<std::shared_ptr<Camera>>(c);
				camera->PreRender();
				camera->Clear();
				camera->Render(casted_renderers);
				camera->PostRender();
			}
		}
	}
	
	void Scene::Begin() {
	
		std::vector<std::any> scripts;
		if (m_Entities.Get(typeid(Script), scripts)) {
			for (auto s : scripts) {
				std::any_cast<std::shared_ptr<Script>>(s)->Begin();
			}
		}
	}
	
	void Scene::Tick() {
	
		std::vector<std::any> scripts;
		if (m_Entities.Get(typeid(Script), scripts)) {
			for (auto s : scripts) {
				std::any_cast<std::shared_ptr<Script>>(s)->Tick();
			}
		}
	
		Draw();
	}
	
	void Scene::FixedTick() {
	
		const glm::vec3 gravity(0.0f, -9.82f, 0.0f);
	
		std::vector<std::any> rigidbodies;
		if (m_Entities.Get(typeid(Rigidbody), rigidbodies)) {
	
			/* FRAME BEGIN */
			for (auto r : rigidbodies) {
	
				auto rigidbody = std::any_cast<std::shared_ptr<Rigidbody>>(r);
	
				const float radius = rigidbody->GetCollider().lock()->Radius();
	
				rigidbody->ClearForce();
	
				// Accumulate forces applied every frame.
				auto force =
					gravity +
					rigidbody->DragForce(radius);
	
				// Apply the forces to the rigidbody.
				rigidbody->AddForce(force);
			}
	
			/* COLLISION */
			for (auto r : rigidbodies) {
	
				auto rigidbody    = std::any_cast<std::shared_ptr<Rigidbody>>(r);
				auto thisCollider = rigidbody->GetCollider().lock();
	
				std::vector<std::any> colliders;
				if (m_Entities.Get(typeid(Collider), colliders)) {
	
					for (auto c : colliders) {
	
						auto otherCollider = std::any_cast<std::shared_ptr<Collider>>(c);
	
						Collision hit;
						if (thisCollider->Evaluate(otherCollider, &hit)) {
	
							rigidbody->GetTransform().lock()->m_Position += hit.ContactPoint();
	
							// Accumulate forces created by this collision.
							auto force =
								hit.Impulse() +
								rigidbody->FrictionForce(hit);
							
							// Apply the forces to the rigidbody.
							// These forces happen 'instantly' so we cancel the delta.
							rigidbody->AddForce(force / Time::FixedDeltaTime());
						}
					}
				}
			}
	
			/* FRAME END */
			for (auto r : rigidbodies) {
	
				auto rigidbody = std::any_cast<std::shared_ptr<Rigidbody>>(r);
	
				rigidbody->Euler(Time::FixedDeltaTime());
			}
		}
	}
	
	Hashmap<std::type_index, std::vector<std::any>> Scene::Entities() {
		return m_Entities;
	}
	
	void Scene::Save(const std::filesystem::path& _path) {
	
		std::cout << "Saving Scene... ";
	
		{
			auto ofStream = std::ofstream(_path);
	
			auto xml = cereal::XMLOutputArchive(ofStream);
	
			auto map = Entities().GetAll();
	
			xml.setNextName("Entities"); // Start "Entities"
			xml.startNode();
	
			for (auto& category : map) {
	
				auto& entries = category.second;
	
				for (auto& entry : entries) {
	
					if (category.first == typeid(GameObject)) { // Only serialise things attached to GameObject.
	
						auto entity = std::any_cast<std::shared_ptr<GameObject>>(entry);
	
						auto subCategories = entity->Components().GetAll();
	
						xml.setNextName(entity->Name().c_str());
						xml.startNode();
	
						for (auto& subCategory : subCategories) {
	
							auto& components = subCategory.second;
	
							if (subCategory.first == typeid(Transform)) {		// Serialise Transform.
	
								for (auto& component : components) {
	
									auto transform = std::any_cast<std::shared_ptr<Transform>>(component);
	
									xml.setNextName("Transform");
									xml.startNode();
	
									xml(cereal::make_nvp("Position", Serialisation::Serialise(transform->m_Position)));
									xml(cereal::make_nvp("Rotation", Serialisation::Serialise(transform->m_Rotation)));
									xml(cereal::make_nvp("Scale",    Serialisation::Serialise(transform->m_Scale)));
	
									xml.finishNode();
								}
							}
							else if (subCategory.first == typeid(Rigidbody)) {	// Serialise Rigidbody.
	
								for (auto& component : components) {
	
									auto rigidbody = std::any_cast<std::shared_ptr<Rigidbody>>(component);
	
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
							else if (subCategory.first == typeid(Camera)) {		// Serialise Camera.
	
								for (auto& component : components) {
	
									auto camera = std::any_cast<std::shared_ptr<Camera>>(component);
	
									xml.setNextName("Camera");
	
									xml.startNode();
									xml.finishNode();
								}
							}
							else if (subCategory.first == typeid(Collider)) {	// Serialise Collider.
	
								for (auto& component : components) {
	
									auto collider = std::any_cast<std::shared_ptr<Collider>>(component);
	
									xml.setNextName("Collider");
	
									xml.startNode();
									xml(cereal::make_nvp("Type", std::to_string((int)collider->GetType())));
									xml.finishNode();
								}
							}
							else if (subCategory.first == typeid(Light)) {		// Serialise Light.
	
								for (auto& component : components) {
	
									auto light = std::any_cast<std::shared_ptr<Light>>(component);
	
									xml.setNextName("Light");
	
									xml.startNode();
									xml.finishNode();
								}
							}
							else if (subCategory.first == typeid(Renderer)) {	// Serialise Renderer.
	
								for (auto& component : components) {
	
									auto renderer = std::any_cast<std::shared_ptr<Renderer>>(component);
	
									xml.setNextName("Renderer");
	
									xml.startNode();
									xml.finishNode();
								}
							}
							else if (subCategory.first == typeid(Script)) {		// Serialise Shader.
	
								xml.setNextName("Script");
								xml.startNode();
	
								for (auto& component : components) {
									xml(cereal::make_nvp("Type", std::string(component.type().name())));
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
	
		std::cout << "Loading Scene: " << _path << "\n";
	
		auto result = std::make_shared<Scene>();
		
		auto ifStream = std::ifstream(_path, std::ifstream::in);
	
		auto xml = cereal::XMLInputArchive(ifStream);
	
		xml.startNode();
	
		int gameObjectCount = 0;
		xml.loadSize(gameObjectCount);
	
		std::cout << "GameObjects: " << gameObjectCount << "\n";
	
		for (size_t i = 0; i < gameObjectCount; i++) {
	
			auto gameObjectName = xml.getNodeName();
			std::cout << "\tName: \"" << gameObjectName << "\"\n";
	
			// Create GameObject to populate.
			auto gameObject = GameObject::Create(result->shared_from_this(), gameObjectName);
			result->Attach(gameObject);
	
			xml.startNode();
	
			int componentCount = 0;
			xml.loadSize(componentCount);
	
			std::cout << "\tComponents: " << componentCount << "\n";
	
			for (size_t j = 0; j < componentCount; j++) {
	
				auto componentName = std::string(xml.getNodeName());
				std::cout << "\t\t" << componentName << "\n";
	
				if (componentName == "Transform") {			// Deserialise Transform.
	
					auto transform = gameObject->AddComponent<Transform>();
	
					xml.startNode();
	
					transform->m_Position = Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, 3));
					transform->m_Rotation = Serialisation::Deserialise<glm::quat>(Serialisation::ParseNext(xml, 3));
					transform->m_Scale    = Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, 3));
	
					xml.finishNode();
				}
				else if (componentName == "Rigidbody") {	// Deserialise Rigidbody.
	
					auto rigidbody = gameObject->AddComponent<Rigidbody>();
	
					xml.startNode();
	
					rigidbody->       Velocity(Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, 3)));
					rigidbody->AngularVelocity(Serialisation::Deserialise<glm::vec3>(Serialisation::ParseNext(xml, 3)));
					rigidbody->           Mass(Serialisation::Deserialise<float>    (Serialisation::ParseNext(xml, 3)));
					rigidbody->           Drag(Serialisation::Deserialise<float>    (Serialisation::ParseNext(xml, 3)));
					rigidbody->    AngularDrag(Serialisation::Deserialise<float>    (Serialisation::ParseNext(xml, 3)));
					
					xml.finishNode();
				}
				else if (componentName == "Camera") {		// Deserialise Camera.
				
					auto camera = gameObject->AddComponent<Camera>();
	
					xml.startNode();
					xml.finishNode();
				}
				else if (componentName == "Collider") {		// Deserialise Collider.
	
					auto collider = gameObject->AddComponent<Collider>();
	
					xml.startNode();
	
					std::string type;
					xml(type);
	
					collider->SetType((Collider::Type)std::stoi(type));
	
					xml.finishNode();
				}
				else if (componentName == "Light") {		// Deserialise Light
					
					auto light = gameObject->AddComponent<Light>();
	
					xml.startNode();
					xml.finishNode();
				}
				else if (componentName == "Renderer") {		// Deserialise Renderer.
	
					auto renderer = gameObject->AddComponent<Renderer>();
	
					xml.startNode();
					xml.finishNode();
				}
				else if (componentName == "Script") {		// Deserialise Scripts...
	
					std::shared_ptr<Script> script;
	
					xml.startNode();
	
					std::string type;
					xml(type);
	
					std::cout << "\t\t\t\"" << type << "\"\n";
	
					if (type == "class std::shared_ptr<class Player>") {		// Player
						script = gameObject->AddComponent<Player>();
					}
					else if (type == "class std::shared_ptr<class Ball>") {		// Ball
						script = gameObject->AddComponent<Ball>();
					}
					else if (type == "class std::shared_ptr<class Plane>") {	// Plane
						script = gameObject->AddComponent<Plane>();
					}
					else if (type == "class std::shared_ptr<class OrbitCam>") {	// OrbitCam
						script = gameObject->AddComponent<OrbitCam>();
					}
					else {
						Serialisation::NotImplementedException(type);
					}
	
					result->Attach(script);
	
					xml.finishNode();
				}
			}
	
			xml.finishNode();
		}
	
		xml.finishNode();
	
		std::cout << "Finished Loading." << std::endl;
	
		return result;
	}
}