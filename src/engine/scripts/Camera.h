#pragma once

#ifndef _CAMERA_H
#define _CAMERA_H

#include "Renderer.h"
#include "Window.h"

class Camera : public Component {

friend class Window;

private:

	/// <summary> Window of the Camera. </summary>
	std::shared_ptr<Window>    m_Window;

	/// <summary> Transform of the Camera. </summary>
	std::shared_ptr<Transform> m_Transform;

	float m_FOV;
	float m_NearClip;
	float m_FarClip;

	glm::mat4 m_Projection;

	bool m_IsDirty;
	
public:

	 Camera(std::weak_ptr<GameObject> _parent);
	~Camera();

	/// <summary> Render clear the camera. </summary>
	void Clear();

	/// <summary> Draw the Renderer using the Camera. </summary>
	void Draw(std::weak_ptr<Renderer> _renderer);

	/// <summary> Set the Camera's Window. </summary>
	void SetWindow(std::weak_ptr<Window> _window);
	
	/// <summary> Get the Camera's Window. </summary>
	std::weak_ptr<Window> GetWindow();

	/// <summary> Set the Camera's Transform. </summary>
	void SetTransform(std::weak_ptr<Transform> _transform);
	
	/// <summary> Get the Camera's Transform. </summary>
	std::weak_ptr<Transform> GetTransform();

	/// <summary> Get the Camera's Aspect. </summary>
	float Aspect();

	/// <summary> Set the Camera's field of view. </summary>
	void FOV(float _fov);

	/// <summary> Get the Camera's field of view. </summary>
	float FOV();

	/// <summary> Set the Camera's near clip plane. </summary>
	void NearClip(float _nearClip);
	
	/// <summary> Get the Camera's near clip plane. </summary>
	float NearClip();

	/// <summary> Set the Camera's far clip plane. </summary>
	void FarClip(float _farClip);
	
	/// <summary> Get the Camera's far clip plane. </summary>
	float FarClip();

	/// <summary> Set the Camera's clear color. </summary>
	void ClearColor(glm::vec4 _color);
	
	/// <summary> Get the Camera's clear color. </summary>
	glm::vec4 ClearColor();

	/// <summary> Get the Camera's projection matrix. </summary>
	const glm::mat4& Projection();

	/// <summary> Set the Camera dirty, so that it computes a new projection matrix. </summary>
	void SetDirty();
};

#endif // !_CAMERA_H
