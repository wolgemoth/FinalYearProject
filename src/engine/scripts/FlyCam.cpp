#include "stdafx.h"

#include "FlyCam.h"

#include "Cursor.h"
#include "Input.h"
#include "Resources.h"
#include "Settings.h"

// @Assessor: Please mark this class for GEP.

namespace LouiEriksson {
	
	FlyCam::FlyCam(const std::shared_ptr<GameObject>& _parent) : Script(_parent) {
	
		m_Camera    = std::shared_ptr<Camera>   (nullptr);
		m_Transform = std::shared_ptr<Transform>(nullptr);
	
		m_Motion   = glm::vec3(0.0f);
		m_Rotation = glm::vec3(0.0f);
		
		m_MoveSpeed =  5.0f;
		m_LookSpeed = 30.0f;
		
		m_AnimationProgress = 0.0f;
	}
	
	FlyCam::~FlyCam() = default;
	
	std::shared_ptr<Camera> FlyCam::GetCamera() {
		return m_Camera.lock();
	}
	
	std::shared_ptr<Transform> FlyCam::GetTransform() {
		return m_Transform.lock();
	}
	
	void FlyCam::Begin() {
		
		auto scene = Parent()->GetScene();
		
		// Get or add Transform.
		// TODO: Do this better:
		m_Transform = Parent()->GetComponent<Transform>();
		if (m_Transform.expired()) {
			m_Transform = Parent()->AddComponent<Transform>();
		}
		
		// Get or add Camera.
		// TODO: Do this better:
		m_Camera = scene->Attach(Parent()->AddComponent<Camera>());
		if (m_Camera.expired()) {
			m_Camera = Parent()->AddComponent<Camera>();
		}
		
		// Get or add AudioListener.
		// TODO: Do this better:
		m_AudioListener = Parent()->GetComponent<AudioListener>();
		if (m_AudioListener.expired()) {
			m_AudioListener = Parent()->AddComponent<AudioListener>();
		}
		
		m_AudioListener.lock()->Init();
		
		// Update the camera's parameters to match the ones in Settings.
		SyncCameraSettings();
		
		// Set camera defaults:
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
	
	void FlyCam::Tick() {
		
		// Update the camera's parameters to match the ones in Settings.
		SyncCameraSettings();
		
		// Lock the cursor to the center of the screen if the window is in focus:
		if (GetCamera()->GetWindow()->Focused()) {
			Cursor::SetState({ Cursor::State::LockMode::Centered, false });
		}
		
		/* LOOK & MOVE */
		{
			glm::vec3 movement_input;
			glm::vec3    mouse_input;
			
			// Handle look and move input. Disable if cursor is not centered.
			if (Cursor::GetState().m_LockMode == Cursor::State::LockMode::Centered) {
				
				movement_input = glm::vec3(
					static_cast<float>(Input::Key::Get(SDL_SCANCODE_A     ) - Input::Key::Get(SDL_SCANCODE_D    )),
					static_cast<float>(Input::Key::Get(SDL_SCANCODE_LSHIFT) - Input::Key::Get(SDL_SCANCODE_LCTRL)),
					static_cast<float>(Input::Key::Get(SDL_SCANCODE_W     ) - Input::Key::Get(SDL_SCANCODE_S    ))
				);
				
				mouse_input = glm::vec3(
					 Input::Mouse::Motion().y,
					 Input::Mouse::Motion().x,
					 0.0f
				);
			}
			else {
				movement_input = glm::vec3(0.0f);
				   mouse_input = glm::vec3(0.0f);
			}
			
			// Rotate the camera:
			{
				m_Rotation = Utils::WrapAngle(m_Rotation + (mouse_input * Time::DeltaTime() * m_LookSpeed));
				m_Rotation.x = glm::clamp(m_Rotation.x, -89.999f, 89.999f);
				
				const auto direction = glm::vec3(
					glm::cos(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x)),
					glm::sin(glm::radians(m_Rotation.x)),
					glm::sin(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x))
				);
				
				GetTransform()->m_Rotation = glm::quat(glm::lookAt(VEC_ZERO, direction, VEC_UP));
			}
			
			// Move the player:
			{
				m_Motion = glm::mix(
					m_Motion,
					movement_input * m_MoveSpeed * GetTransform()->m_Rotation,
					Time::DeltaTime() * 5.0f
				);
				
				GetTransform()->m_Position += (m_Motion * Time::DeltaTime());
			}
		}
		
		// Cool slow motion effect when 'P' is held:
		{
			const auto targetScale = Input::Key::Get(SDL_SCANCODE_P) ? 0.0f : 1.0f;
			const auto effectSpeed = 3.0f;
			
			Time::Scale(glm::mix(Time::Scale(), targetScale, Time::UnscaledDeltaTime() * effectSpeed));
		}
		
		m_AudioListener.lock()->Tick();
	}
	
	void FlyCam::SyncCameraSettings() {
		
		GetCamera()->FOV     (Settings::Graphics::Perspective::s_FOV);
		GetCamera()->NearClip(Settings::Graphics::Perspective::s_NearClip);
		GetCamera()->FarClip (Settings::Graphics::Perspective::s_FarClip);
	}
}