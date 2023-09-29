#include "stdafx.h"

#include "Player.h"

Player::Player(std::weak_ptr<GameObject> _parent) : Script(_parent) {}
Player::~Player() {}

void Player::Begin() {

	auto scene = Component::GetGameObject().lock()->GetScene();
	
	// Create an initialise curuthers object.
	curuthers = scene.lock()->Attach(GameObject::Create(scene, "Curuthers")); {

		// Compile shader.
		auto diffuse = Shader::Create({
			{ "shaders/diffuse/diffuse.vert", GL_VERTEX_SHADER   },
			{ "shaders/diffuse/diffuse.frag", GL_FRAGMENT_SHADER }
		});
		diffuse->BindAttribute(0, "a_Position");
		diffuse->BindAttribute(1, "a_TexCoord");

		// Load mesh.
		auto mesh = Mesh::Load("models/curuthers/curuthers.obj");

		// Create material from shader.
		auto material = Material::Create(diffuse);
		material->Texture_ID(mesh->Texture_ID());

		auto transform = curuthers.lock()->AddComponent<Transform>().lock();
		transform->m_Position = glm::vec3(0.0f, 0.0f, -7.0f);

		auto renderer = scene.lock()->Attach(curuthers.lock()->AddComponent<Renderer>().lock());
		renderer->SetMesh(mesh);
		renderer->SetMaterial(material);
		renderer->SetTransform(transform);
	}

	// Create and initialise camera object.
	auto cameraObject = scene.lock()->Attach(GameObject::Create(scene, "Camera")); {

		camera = scene.lock()->Attach(cameraObject->AddComponent<Camera>().lock()); {

			auto transform = cameraObject->AddComponent<Transform>().lock();
			transform->m_Position = glm::vec3(0.0f, 0.0f, 0.0f);

			camera.lock()->SetTransform(transform);
			camera.lock()->SetWindow(Window::Get(1));
		}
	}
}

void Player::Tick() {

	/* MOVE & ROTATE CAMERA */

	camera.lock()->GetTransform().lock()->m_Position = glm::vec3(sin(Time::Elapsed()) * 4.0f, 0.0f, 0.0f);

	camera.lock()->GetTransform().lock()->m_Rotation =
		glm::quat(
			glm::radians(
				glm::vec3(0.0f, sin(Time::Elapsed()) * 10.0f, sin(Time::Elapsed()) * 10.0f)
			)
		);

	/* MOVE AND ROTATE TRANSFORM */

	curuthers.lock()->GetComponent<Transform>().lock()->m_Rotation *=
		glm::quat(
			glm::radians(
				glm::vec3(45.0f, 0.0f, 0.0f) * Time::DeltaTime()
			)
		);
}
