#include "stdafx.h"

#include "Player.h"

namespace LouiEriksson {
	
	Player::Player(std::weak_ptr<GameObject> _parent) : Script(std::move(_parent)) {}
	Player::~Player() = default;
	
	void Player::Begin() {
	
		auto scene = Component::GetGameObject().lock()->GetScene();
		
		// Create an initialise m_Curuthers object.
		m_Curuthers = scene.lock()->Attach(GameObject::Create(scene, "Curuthers")); {
	
			// Compile shader.
			auto diffuse = Shader::Create({
				{ "shaders/diffuse/diffuse.vert", GL_VERTEX_SHADER   },
				{ "shaders/diffuse/diffuse.frag", GL_FRAGMENT_SHADER }
			});
			diffuse->BindAttribute(0, "a_Position");
			diffuse->BindAttribute(1, "a_TexCoord");
	
			// Load mesh.
			auto mesh = Mesh::Load("models/m_Curuthers/m_Curuthers.obj");
	
			// Create material from shader.
			auto material = Material::Create(diffuse);
			material->Texture_ID(mesh->Texture_ID());
	
			auto transform = m_Curuthers.lock()->AddComponent<Transform>().lock();
			transform->m_Position = glm::vec3(0.0f, 0.0f, -7.0f);
	
			auto renderer = scene.lock()->Attach(m_Curuthers.lock()->AddComponent<Renderer>().lock());
			renderer->SetMesh(mesh);
			renderer->SetMaterial(material);
			renderer->SetTransform(transform);
		}
	
		// Create and initialise camera object.
		auto cameraObject = scene.lock()->Attach(GameObject::Create(scene, "Camera")); {
			
			m_Camera = scene.lock()->Attach(cameraObject->AddComponent<Camera>().lock()); {
	
				auto transform = cameraObject->AddComponent<Transform>().lock();
				transform->m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	
				m_Camera.lock()->SetTransform(transform);
				m_Camera.lock()->SetWindow(Window::Get(1));
			}
		}
	}
	
	void Player::Tick() {
	
		/* MOVE & ROTATE CAMERA */
	
		m_Camera.lock()->GetTransform().lock()->m_Position = glm::vec3(std::sin(Time::Elapsed()) * 4.0f, 0.0f, 0.0f);
		
		m_Camera.lock()->GetTransform().lock()->m_Rotation =
			glm::quat(
				glm::radians(
					glm::vec3(0.0f, std::sin(Time::Elapsed()) * 10.0f, std::sin(Time::Elapsed()) * 10.0f)
				)
			);
	
		/* MOVE AND ROTATE TRANSFORM */
	
		m_Curuthers.lock()->GetComponent<Transform>().lock()->m_Rotation *=
			glm::quat(
				glm::radians(
					glm::vec3(45.0f, 0.0f, 0.0f) * Time::DeltaTime()
				)
			);
	}
}