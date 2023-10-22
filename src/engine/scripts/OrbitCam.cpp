#include "stdafx.h"

#include "OrbitCam.h"

namespace LouiEriksson {
	
	OrbitCam::OrbitCam(const std::shared_ptr<GameObject>& _parent) : Script(_parent) {
	
		m_Camera    = std::shared_ptr<Camera>   (nullptr);
		m_Transform = std::shared_ptr<Transform>(nullptr);
	
		m_Target = glm::vec3(0.0f);
	
		m_OrbitSpeed    =  5.0f;
		m_OrbitDistance = 10.0f;
	}
	
	OrbitCam::~OrbitCam() = default;
	
	std::shared_ptr<Camera> OrbitCam::GetCamera() {
		return m_Camera.lock();
	}
	
	std::shared_ptr<Transform> OrbitCam::GetTransform() {
		return m_Transform.lock();
	}
	
	void OrbitCam::Target(const glm::vec3 _target) {
		m_Target = _target;
	}
	const glm::vec3& OrbitCam::Target() {
		return m_Target;
	}
	
	void OrbitCam::FOV(const float& _fov) {
		m_Camera.lock()->FOV(_fov);
	}
	const float& OrbitCam::FOV() {
		return m_Camera.lock()->FOV();
	}
	
	void OrbitCam::Begin() {
		
		auto scene = Parent()->GetScene();
		
		// Get or add Transform.
		m_Transform = Parent()->GetComponent<Transform>();
		if (m_Transform.expired()) {
			m_Transform = Parent()->AddComponent<Transform>();
		}
		
		GetTransform()->m_Position = glm::vec3(0.0f, 5.0f, -7.0f);
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
		
		GetCamera()->SetWindow(Window::Get(2));
		GetCamera()->SetTransform(GetTransform());
		GetCamera()->ClearColor(glm::vec4(0.0f)); //glm::vec4(0.34f, 0.764f, 1.0f, 0.0f)
	}
	
	void OrbitCam::Tick() {
	
	}
}