#include "stdafx.h"

#include "Camera.h"

Camera::Camera(std::weak_ptr<GameObject> _parent) : Component(_parent) {
	
	m_Window    = std::shared_ptr<Window>(nullptr);
	m_Transform = std::shared_ptr<Transform>(nullptr);

	m_FOV      = 90.0f;
	m_NearClip = 0.1f;
	m_FarClip  = 1000.0f;

	m_Projection = glm::mat4(1.0f);
	m_IsDirty = true;
}

Camera::~Camera() {

	/* Attempt to unlink the camera from the window.
	   In the case something catastrophic happens, 
	   log the error. */
	try {
		if (m_Window != nullptr) {
			m_Window->Unlink(*this);
		}
	}
	catch (std::exception e) {
		std::cout << e.what() << "\n";
	}
}

void Camera::Clear() {

	// Clear the camera.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Camera::Draw(std::weak_ptr<Renderer> _renderer) {

	glEnable(GL_DEPTH_TEST);

	// Lock a reference to the renderer for easy access.
	auto r = _renderer.lock();

	// Get references to various components needed for rendering.
	auto transform = r->GetTransform().lock();
	auto material  = r->GetMaterial().lock();
	auto mesh      = r->GetMesh().lock();

	glUseProgram(material->GetShader()->ID());

	/* PROJECTION */ glUniformMatrix4fv(r->m_ProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(Projection()));
	/* MODEL      */ glUniformMatrix4fv(r->m_ModelMatrixID,      1, GL_FALSE, glm::value_ptr(glm::inverse(m_Transform->TRS()) * transform->TRS()));

	glBindVertexArray(mesh->VAO_ID());
	glBindTexture(GL_TEXTURE_2D, material->Texture_ID());

	glDrawArrays(GL_TRIANGLES, 0, mesh->VertexCount());

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_DEPTH_TEST);
}

void Camera::SetWindow(std::weak_ptr<Window> _window) {
	_window.lock()->Link(*this);
}
std::weak_ptr<Window> Camera::GetWindow() {
	return m_Window;
}

void Camera::SetTransform(std::weak_ptr<Transform> _transform) {
	m_Transform = _transform.lock();
}
std::weak_ptr<Transform> Camera::GetTransform() {
	return m_Transform;
}

float Camera::Aspect() {
	return m_Window->Aspect();
}

void Camera::FOV(float _fov) {
	m_IsDirty = true; // Set dirty.

	m_FOV = _fov; 
}
float Camera::FOV() {
	return m_FOV; 
}

void Camera::NearClip(float _nearClip) {
	m_IsDirty = true; // Set dirty.

	m_NearClip = _nearClip; 
}
float Camera::NearClip() {
	return m_NearClip; 
}

void Camera::FarClip(float _farClip) {
	m_IsDirty = true; // Set dirty.

	m_FarClip = _farClip; 
}
float Camera::FarClip() {
	return m_FarClip; 
}

void Camera::ClearColor(glm::vec4 _color) {

	// Set the clear color to the provided.
	glClearColor(_color[0], _color[1], _color[2], _color[3]);
}
glm::vec4 Camera::ClearColor() {

	glm::vec4 result;

	// Read the clear value from opengl into the result.
	glGetFloatv(GL_COLOR_CLEAR_VALUE, &result[0]);

	return result;
}

const glm::mat4& Camera::Projection() {

	// Recalculate the perspective matrix if the camera is dirtied.
	if (m_IsDirty == true) {
		m_IsDirty = false;
		
		m_Projection = glm::perspective(glm::radians(m_FOV), Aspect(), m_NearClip, m_FarClip);
	}

	return m_Projection; 
}

void Camera::SetDirty() {
	m_IsDirty = true;
}
