#include "stdafx.h"

#include "OrbitCam.h"

#include "Settings.h"

// @Assessor: This class has changed significantly since it was submitted for 3DGP. Please mark it for GACP.

namespace LouiEriksson {
	
	OrbitCam::OrbitCam(const std::shared_ptr<GameObject>& _parent) : Script(_parent) {
	
		m_Camera    = std::shared_ptr<Camera>   (nullptr);
		m_Transform = std::shared_ptr<Transform>(nullptr);
	
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
	}
	
	void OrbitCam::Tick() {
		
		GetCamera()->FOV     (Settings::Graphics::Perspective::s_FOV);
		GetCamera()->NearClip(Settings::Graphics::Perspective::s_NearClip);
		GetCamera()->FarClip (Settings::Graphics::Perspective::s_FarClip);
		
		using target = Settings::Graphics::Perspective::Orbit;
		
		if (target::s_Enabled) {
			
			float distance = target::s_Offset.z + glm::sin(m_AnimationProgress * target::s_Speed.z) * target::s_Amount.z;
			
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