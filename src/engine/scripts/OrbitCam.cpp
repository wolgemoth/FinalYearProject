#include "stdafx.h"

#include "OrbitCam.h"

OrbitCam::OrbitCam(std::weak_ptr<GameObject> _parent) : Script(_parent) {

	m_Camera    = std::shared_ptr<Camera>   (nullptr);
	m_Transform = std::shared_ptr<Transform>(nullptr);

	m_Target = glm::vec3(0.0f);

	m_OrbitSpeed    =  5.0f;
	m_OrbitDistance = 10.0f;
}

OrbitCam::~OrbitCam() {}

std::weak_ptr<Camera> OrbitCam::GetCamera() {
	return m_Camera;
}

std::weak_ptr<Transform> OrbitCam::GetTransform() {
	return m_Transform;
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

	auto parent = GetGameObject().lock();

	auto scene = parent->GetScene().lock();

	// Get or add Transform.
	m_Transform = parent->GetComponent<Transform>().lock();
	if (m_Transform.expired()) {
		m_Transform = parent->AddComponent<Transform>().lock();
	}

	m_Transform.lock()->m_Position = glm::vec3(0.0f, 5.0f, 7.0f);
	m_Transform.lock()->m_Rotation = glm::quat(
		glm::radians(
			glm::vec3(35.0f, 0.0f, 0.0f)
		)
	);

	// Get or add Camera.
	m_Camera = scene->Attach(parent->AddComponent<Camera>().lock());
	if (m_Camera.expired()) {
		m_Camera = parent->AddComponent<Camera>().lock();
	}

	m_Camera.lock()->SetWindow(Window::Get(1));
	m_Camera.lock()->SetTransform(m_Transform);
	m_Camera.lock()->ClearColor(glm::vec4(0.34f, 0.764f, 1.0f, 0.0f));
}

void OrbitCam::Tick() {

}