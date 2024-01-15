#include "OrbitCam.h"

// @Assessor: This class has changed significantly since it was submitted for 3DGP. Please mark it for GACP.

namespace LouiEriksson::Game {
	
	OrbitCam::OrbitCam(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : Script(_parent),
			           m_Target(0.0f),
			m_AnimationProgress(0.0f) {}
	
	OrbitCam::~OrbitCam() = default;
	
	void OrbitCam::Begin() {
		
		if (const auto p =      Parent().lock()) {
		if (const auto s = p->GetScene().lock()) {
			
			// Get Transform.
			m_Transform = p->GetComponent<Transform>();
			
			if (const auto t = m_Transform.lock()) {
				
				t->m_Position = glm::vec3(0.0f, 2.5f, -7.0f);
				t->m_Rotation = glm::quat(
					glm::radians(
						glm::vec3(-35.0f, 0.0f, 0.0f)
					)
				);
				
				// Add Camera.
				m_Camera = p->AddComponent<Graphics::Camera>();
				
				// Update the camera's parameters to match the ones in Settings.
				SyncCameraSettings();
				
				if (const auto c = m_Camera.lock()) {
					c->SetWindow(Window::Get(2));
					c->SetTransform(t);
					c->ClearColor(glm::vec4(0.0f));
				}
				
				// Add a light to the scene for testing purposes.
				// TODO: Add a light to the scene through the scene's file, not code.
				{
					const auto light_gameObject = ECS::GameObject::Create(s->shared_from_this(), "Light");
					light_gameObject->AddComponent<Transform>();
					light_gameObject->AddComponent<Graphics::Light>();
				}
			}
		}}
	}
	
	void OrbitCam::Tick() {
		
		SyncCameraSettings();
		
		using target = Settings::Graphics::Perspective::Orbit;
		
		if (target::s_Enabled) {
			
			if (const auto t = m_Transform.lock()) {
				
				const auto distance = target::s_Offset.z + glm::sin(m_AnimationProgress * target::s_Speed.z) * target::s_Amount.z;
				
				t->m_Position = glm::vec3(
					glm::sin(m_AnimationProgress * target::s_Speed.x) * distance,
					target::s_Offset.y + glm::sin(m_AnimationProgress * target::s_Speed.y) * target::s_Amount.y,
					glm::cos(m_AnimationProgress * target::s_Speed.x) * distance
				);
				
				t->m_Rotation = glm::lookAt(glm::vec3(0.0f), t->m_Position, VEC_UP);
			
				m_AnimationProgress += Time::DeltaTime();
			}
		}
	}
	
	void OrbitCam::SyncCameraSettings() noexcept {
		
		if (const auto c = m_Camera.lock()) {
			
			c->FOV     (Settings::Graphics::Perspective::s_FOV);
			c->NearClip(Settings::Graphics::Perspective::s_NearClip);
			c->FarClip (Settings::Graphics::Perspective::s_FarClip);
		}
	}
	
} // LouiEriksson::Game