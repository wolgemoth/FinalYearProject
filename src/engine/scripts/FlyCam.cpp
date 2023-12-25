#include "stdafx.h"

#include "FlyCam.h"

#include "Settings.h"
#include "Input.h"

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
		m_Transform = Parent()->GetComponent<Transform>();
		if (m_Transform.expired()) {
			m_Transform = Parent()->AddComponent<Transform>();
		}
		
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
	
	void FlyCam::Tick() {
		
		GetCamera()->FOV     (Settings::Graphics::Perspective::s_FOV);
		GetCamera()->NearClip(Settings::Graphics::Perspective::s_NearClip);
		GetCamera()->FarClip (Settings::Graphics::Perspective::s_FarClip);
		
		const auto movement_input = glm::vec3(
			static_cast<float>(Input::Key::Get(SDL_SCANCODE_A     ) - Input::Key::Get(SDL_SCANCODE_D    )),
			static_cast<float>(Input::Key::Get(SDL_SCANCODE_LSHIFT) - Input::Key::Get(SDL_SCANCODE_LCTRL)),
			static_cast<float>(Input::Key::Get(SDL_SCANCODE_W     ) - Input::Key::Get(SDL_SCANCODE_S    ))
		);
		
		m_Rotation = Utils::WrapAngle(
			m_Rotation +
				glm::vec3(
					 Input::Mouse::Motion().y,
					 Input::Mouse::Motion().x,
					 0.0f
				) * Time::DeltaTime() * m_LookSpeed
		);
		
		m_Rotation.x = glm::clamp(m_Rotation.x, -89.999f, 89.999f);
		
		const auto direction = glm::vec3(
			glm::cos(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x)),
			glm::sin(glm::radians(m_Rotation.x)),
			glm::sin(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x))
		);
		
		GetTransform()->m_Rotation = glm::quat(glm::lookAt(VEC_ZERO, direction, VEC_UP));

		m_Motion = glm::mix(
			m_Motion,
			movement_input * m_MoveSpeed * GetTransform()->m_Rotation,
			Time::DeltaTime() * 5.0f
		);
		
		GetTransform()->m_Position += (m_Motion * Time::DeltaTime());
	}
}