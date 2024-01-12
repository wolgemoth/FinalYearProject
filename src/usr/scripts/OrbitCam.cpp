#include "OrbitCam.h"

#include "../../engine/scripts/core/Script.h"
#include "../../engine/scripts/core/Settings.h"
#include "../../engine/scripts/core/Time.h"
#include "../../engine/scripts/core/Transform.h"
#include "../../engine/scripts/ecs/GameObject.h"
#include "../../engine/scripts/graphics/Camera.h"
#include "../../engine/scripts/graphics/Light.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/trigonometric.hpp>

#include <cmath>
#include <memory>

// @Assessor: This class has changed significantly since it was submitted for 3DGP. Please mark it for GACP.

namespace LouiEriksson {
	
	OrbitCam::OrbitCam(const std::shared_ptr<GameObject>& _parent) : Script(_parent) {
	
		m_Camera    = std::shared_ptr<Camera>   (nullptr);
		m_Transform = std::shared_ptr<Transform>(nullptr);
	
		m_Target = glm::vec3(0.0f);
		
		m_AnimationProgress = 0.0f;
	}
	
	OrbitCam::~OrbitCam() = default;
	
	std::shared_ptr<Camera> OrbitCam::GetCamera() {
		return m_Camera.lock();
	}
	
	std::shared_ptr<Transform> OrbitCam::GetTransform() {
		return m_Transform.lock();
	}
	
	void OrbitCam::Begin() {
		
		auto scene = Parent()->GetScene();
		
		// Get or add Transform.
		m_Transform = Parent()->GetComponent<Transform>();
		if (m_Transform.expired()) {
			m_Transform = Parent()->AddComponent<Transform>();
		}
		
		GetTransform()->m_Position = glm::vec3(0.0f, 2.5f, -7.0f);
		GetTransform()->m_Rotation = glm::quat(
			glm::radians(
				glm::vec3(-35.0f, 0.0f, 0.0f)
			)
		);
		
		// Get or add Camera.
		m_Camera = scene->Attach(Parent()->AddComponent<Camera>());
		if (m_Camera.expired()) {
			m_Camera = Parent()->AddComponent<Camera>();
		}
		
		GetCamera()->FOV     (Settings::Graphics::Perspective::s_FOV);
		GetCamera()->NearClip(Settings::Graphics::Perspective::s_NearClip);
		GetCamera()->FarClip (Settings::Graphics::Perspective::s_FarClip);
		
		GetCamera()->SetWindow(Window::Get(2));
		GetCamera()->SetTransform(GetTransform());
		GetCamera()->ClearColor(glm::vec4(0.0f));
		
		// TODO: Add a light to the scene through the scene's file, not code.
		{
			auto light_gameObject = GameObject::Create(scene->shared_from_this(), "Light");
			scene->Attach(light_gameObject);
		
			// Add a light to the scene for testing purposes.
			auto light_transform = light_gameObject->AddComponent<Transform>();
			auto light = scene->Attach(light_gameObject->AddComponent<Light>());
		}
	}
	
	void OrbitCam::Tick() {
		
		GetCamera()->FOV     (Settings::Graphics::Perspective::s_FOV);
		GetCamera()->NearClip(Settings::Graphics::Perspective::s_NearClip);
		GetCamera()->FarClip (Settings::Graphics::Perspective::s_FarClip);
		
		using target = Settings::Graphics::Perspective::Orbit;
		
		if (target::s_Enabled) {
			
			const auto distance = target::s_Offset.z + glm::sin(m_AnimationProgress * target::s_Speed.z) * target::s_Amount.z;
			
			GetTransform()->m_Position = glm::vec3(
				glm::sin(m_AnimationProgress * target::s_Speed.x) * distance,
				target::s_Offset.y + glm::sin(m_AnimationProgress * target::s_Speed.y) * target::s_Amount.y,
				glm::cos(m_AnimationProgress * target::s_Speed.x) * distance
			);
			
			GetTransform()->m_Rotation = glm::lookAt(glm::vec3(0.0f), GetTransform()->m_Position, VEC_UP);
		
			m_AnimationProgress += Time::DeltaTime();
		}
	}
}