#include "stdafx.h"

#include "Player.h"
#include "Application.h"

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {

	Player::Player(const std::shared_ptr<GameObject>& _parent) : Script(_parent) {
		
		m_NumObstacles = 15;
		
		m_MinPlaneX = -1.0f;
		m_MaxPlaneX = -1.0f;
		
		m_PlayerSpeed = 6.0f;
		m_CameraSpeed = 3.0f;
		  m_GameSpeed = 5.0f;
		
		m_PlayerMoveFreedom = 0.0f;
		
		m_TargetOffset = glm::vec3(0.0f, 4.0f, 0.0f);
		m_CameraOffset = glm::vec3(0.0f, 5.0f, 7.0f);
	}
	
	Player::~Player() = default;
	
	void Player::SpawnPlanes() {
		
		auto scene = Parent()->GetScene();
		
		const float size = 15.0f;
		
		m_PlayerMoveFreedom = size / 2 - 1;
		
		const int noPlanes = 10;
		const int offset = 1;
		const int start = -noPlanes + offset;
		const int end = offset;
		
		m_MinPlaneX = start * size;
		m_MaxPlaneX = end * size;
		
		for (int count = 0, i = start; i < end; ++i, ++count) {
			
			std::stringstream name;
			name << "Floor_" << count;
			
			auto plane = scene->Attach(GameObject::Create(scene, name.str()));
			m_Planes.push_back(plane);
			
			// Compile shader.
			auto surface = Resources::GetShader("pbr");
			
			// Load mesh.
			std::shared_ptr<Mesh> mesh = nullptr;
			File::TryLoad("models/woodfloor/woodfloor.obj", mesh);
			
			// Create material from shader.
			auto material = Resources::GetMaterial("woodfloor");
			
			auto transform = plane->AddComponent<Transform>();
			transform->m_Position = glm::vec3((float)i * size, -2.0f, 0.0f);
			transform->m_Rotation = glm::radians(glm::vec3(0.0f, 180.0f, 0.0f));
			transform->m_Scale = glm::vec3(1.5f);
			
			auto renderer = scene->Attach(plane->AddComponent<Renderer>());
			renderer->SetMesh(mesh);
			renderer->SetMaterial(material);
			renderer->SetTransform(transform);
			
			auto collider = plane->AddComponent<Collider>();
			collider->SetType(Collider::Type::Plane);
			collider->SetTransform(transform);
		}
	}
	
	void Player::SpawnCamera() {
		
		auto scene = Parent()->GetScene();
		
		// Create and initialise camera object.
		m_Camera = scene->Attach(GameObject::Create(scene, "Camera"));
		
		auto cam = scene->Attach(m_Camera.lock()->AddComponent<Camera>());
		
		auto transform = m_Camera.lock()->AddComponent<Transform>();
		transform->m_Position = glm::vec3(7.0f, 5.0f, 0.0f);
		transform->m_Rotation = glm::quat(glm::radians(glm::vec3(0.0f, 90.0f, 0.0f)));
		
		cam->SetTransform(transform);
		cam->SetWindow(Window::Get(2));
		
		const float skyIntensity = 0.0f;//1.2f;
		cam->ClearColor(glm::vec4(0.34f, 0.764f, 1.2f, 0.0f) * skyIntensity);
	}
	
	void Player::SpawnPlayer() {
		
		auto scene = Parent()->GetScene();
		
		// Create an initialise curuthers object.
		m_Player = scene->Attach(GameObject::Create(scene, "Curuthers"));
		
		// Compile shader.
		auto surface = Resources::GetShader("pbr");
		
		// Load mesh.
		std::shared_ptr<Mesh> mesh = nullptr;
		File::TryLoad("models/curuthers/curuthers.obj", mesh);
		
		// Create material from shader.
		auto material = Resources::GetMaterial("curuthers");
		
		auto transform = m_Player.lock()->GetComponent<Transform>();
		if (transform == nullptr) {
			transform = m_Player.lock()->AddComponent<Transform>();
		}
		//transform->m_Position = glm::vec3(0, -1, 0);
		transform->m_Rotation = glm::radians(glm::vec3(0.0f, 90.0f, 0.0f));
		
		auto renderer = scene->Attach(m_Player.lock()->AddComponent<Renderer>());
		renderer->SetMesh(mesh);
		renderer->SetMaterial(material);
		renderer->SetTransform(transform);
	}
	
	void Player::SpawnBalls() {
		
		auto scene = Parent()->GetScene();
		
		for (const auto& ball : m_Obstacles) {
			scene->Detach(ball.lock());
		}
		m_Obstacles.clear();
		
		for (size_t i = 0; i < m_NumObstacles; ++i) {
			
			std::stringstream name;
			name << "Ball_" << i;
			
			auto obstacle = scene->Attach(GameObject::Create(scene, name.str()));
			
			std::shared_ptr<Mesh> mesh = nullptr;
			File::TryLoad("models/sphere/sphere.obj", mesh);
			
			// Compile shader.
			auto surface = Resources::GetShader("pbr");
			
			// Create material from shader.
			auto material = Resources::GetMaterial("sphere");
			
			// Prevents objects spawning too close to the max limit.
			const float obstacleMargin = 20.0f;
			
			// Get or add component.
			auto transform = obstacle->AddComponent<Transform>();
			transform->m_Position = glm::vec3(
				((float)rand() / (float)RAND_MAX) * (m_MinPlaneX + obstacleMargin) - obstacleMargin,
				0,
				((((float)rand() / (float)RAND_MAX) - 0.5f) * 2.0f) * (m_PlayerMoveFreedom + 1.0f / 2.0f)
			);
			
			// Get or add renderer.
			auto renderer = scene->Attach(obstacle->AddComponent<Renderer>());
			
			renderer->SetMesh(mesh);
			renderer->SetMaterial(material);
			renderer->SetTransform(transform);
			
			m_Obstacles.push_back(obstacle);
		}
	}
	
	void Player::Begin() {
		
		auto scene = Parent()->GetScene();
		
		SpawnPlayer();
		SpawnCamera();
		SpawnPlanes();
		SpawnBalls();
	}
	
	void Player::Tick() {
		
		auto player = m_Player.lock()->GetComponent<Transform>();
		
		/* ROTATE CAMERA */
		
		auto cam = m_Camera.lock()->GetComponent<Transform>();
		
		auto mat = glm::lookAtLH(
			cam->m_Position,
			(player->m_Position + m_TargetOffset),
			VEC_UP
		);
		
		auto targetPos = player->m_Position + (player->m_Rotation * m_CameraOffset);
		auto targetRot = glm::quat(mat);
		
		cam->m_Position = glm::mix(cam->m_Position, targetPos, Time::DeltaTime() * m_CameraSpeed);
		cam->m_Rotation = glm::slerp(cam->m_Rotation, targetRot, Time::DeltaTime() * m_CameraSpeed);
		
		//cam->m_Rotation = glm::quat(glm::radians(glm::vec3(-90.0f, 0.0f, 0.0f)));
		
		/* MOVE PLAYER */
		auto direction = static_cast<float>(Input::GetKey(SDL_SCANCODE_A) - Input::GetKey(SDL_SCANCODE_D));
		
		const float dt = Time::DeltaTime();
		
		//m_GameSpeed += dt;
		
		auto l = player->ToWorld(glm::vec3(1, 0, 0));
		
		auto t = player->m_Position + (player->RIGHT * direction * dt * m_PlayerSpeed);
		
		player->m_Position = glm::clamp(
			t,
			-m_PlayerMoveFreedom,
			m_PlayerMoveFreedom
		);
		
		/* MOVE ENVIRONMENT */
		const float& step = Time::DeltaTime() * m_GameSpeed;
		
		for (auto& plane : m_Planes) {
			
			auto planeTransform = plane.lock()->GetComponent<Transform>();
			
			float newPos = planeTransform->m_Position.x + step;
			
			if (newPos > m_MaxPlaneX) {
				newPos -= (m_MaxPlaneX - m_MinPlaneX);
			}
			
			planeTransform->m_Position.x = newPos;
		}
		
		for (auto& obstacle : m_Obstacles) {
			
			auto obstacleTransform = obstacle.lock()->GetComponent<Transform>();
			
			auto dirToPlayer = player->m_Position - obstacleTransform->m_Position;
			
			if (glm::length2(dirToPlayer) < (obstacleTransform->m_Scale.x / 2)) {
				
				std::cout << "You hit a obstacle. Game Over!\n";
				
				Application::Quit();
			}
			
			float newPos = obstacleTransform->m_Position.x + step;
			
			if (newPos > m_MaxPlaneX) {
				newPos -= (m_MaxPlaneX - m_MinPlaneX);
				
				obstacleTransform->m_Position.z = ((((float)rand() / (float)RAND_MAX) - 0.5f) * 2.0f) * (m_PlayerMoveFreedom + 1.0f / 2.0f);
			}
			
			obstacleTransform->m_Position.x = newPos;
		}
	}
}