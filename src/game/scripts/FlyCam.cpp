#include "FlyCam.h"

using namespace LouiEriksson::Engine;

namespace LouiEriksson::Game::Scripts {
	
	FlyCam::FlyCam(const std::weak_ptr<ECS::GameObject>& _parent) : Script(_parent),
			m_Motion(0.0),
			m_MoveSpeed(5.0),
			m_LookSpeed(30.0),
			m_Rotation(0.0) {}
	
	FlyCam::~FlyCam() = default;
	
	void FlyCam::Begin() {
		
		if (const auto p =      Parent().lock()) {
		if (const auto s = p->GetScene().lock()) {
		
			// Get Transform.
			m_Transform = p->GetComponent<Transform>();
			
			// Get or add Camera.
			m_Camera = p->AddComponent<Graphics::Camera>();
			
			// Add AudioListener.
			m_AudioListener = p->AddComponent<Audio::AudioListener>();
			
			// Add AudioSource.
			m_GunSound = p->AddComponent<Audio::AudioSource>();
			if (const auto as = m_GunSound.lock()) {
				as->Clip(Resources::Get<Audio::AudioClip>("machineGun"));
			}
			
			// Update the camera's parameters to match the ones in Settings.
			SyncCameraSettings();
			
			// Set camera defaults:
			if (const auto c =    m_Camera.lock()) {
			if (const auto t = m_Transform.lock()) {
				
				c->SetWindow(Window::Get(2));
				c->SetTransform(t);
				c->ClearColor(glm::vec4(0.0));
			}}
			
			// Add a light to the scene for testing purposes.
			// TODO: Add a light to the scene through the scene's file, not code.
			{
				const auto light_gameObject = ECS::GameObject::Create(s->shared_from_this(), "Light");
				light_gameObject->AddComponent<Transform>();
				light_gameObject->AddComponent<Graphics::Light>();
			}
		}}
		
	}
	
	void FlyCam::Tick() {
		
		// Update the camera's parameters to match the ones in Settings.
		SyncCameraSettings();
		
		if (const auto c =    m_Camera.lock()) {
		if (const auto t = m_Transform.lock()) {
		
			// Lock the cursor to the center of the screen if the window is in focus:
			if (const auto w = c->GetWindow().lock()) {

				if (w->Focused()) {
					Input::Cursor::SetState({ Input::Cursor::State::LockMode::Centered, false });
				}
			}
			
			/* LOOK & MOVE */
			{
				glm::vec3 movement_input;
				glm::vec3    mouse_input;
				
				// Handle look, move, and shoot input. Disable if cursor is not centered.
				if (Input::Cursor::GetState().m_LockMode == Input::Cursor::State::LockMode::Centered) {
					
					mouse_input = glm::vec3(
						 Input::Input::Mouse::Motion().y,
						 Input::Input::Mouse::Motion().x,
						 0.0
					);
					
					mouse_input *= m_Camera.lock()->FOV() * Time::UnscaledDeltaTime<scalar_t>();
					
					movement_input = glm::vec3(
						static_cast<float>(static_cast<int>(Input::Input::Key::Get(SDL_SCANCODE_A     )) - static_cast<int>(Input::Input::Key::Get(SDL_SCANCODE_D    ))),
						static_cast<float>(static_cast<int>(Input::Input::Key::Get(SDL_SCANCODE_LSHIFT)) - static_cast<int>(Input::Input::Key::Get(SDL_SCANCODE_LCTRL))),
						static_cast<float>(static_cast<int>(Input::Input::Key::Get(SDL_SCANCODE_W     )) - static_cast<int>(Input::Input::Key::Get(SDL_SCANCODE_S    )))
					);
					
					if (Input::Input::Mouse::GetDown(SDL_BUTTON_LEFT)) {
						m_GunSound.lock()->Stop();
						m_GunSound.lock()->Play();
					}
				}
				else {
					   mouse_input = glm::vec3(0.0);
					movement_input = glm::vec3(0.0);
				}
				
				// Rotate the camera:
				{
					m_Rotation = Utils::WrapAngle(m_Rotation + (mouse_input * m_LookSpeed));
					m_Rotation.x = std::clamp(m_Rotation.x, static_cast<scalar_t>(-89.999), static_cast<scalar_t>(89.999));
					
					const auto direction = glm::vec3(
						glm::cos(glm::radians(m_Rotation.y)) * glm::cos(glm::radians(m_Rotation.x)),
						glm::sin(glm::radians(m_Rotation.x)),
						glm::sin(glm::radians(m_Rotation.y)) * glm::cos(glm::radians(m_Rotation.x))
					);
					
					t->Rotation(glm::quat(glm::lookAt(VEC_ZERO, direction, VEC_UP)));
				}
				
				// Move the player:
				{
					m_Motion = glm::mix(
						m_Motion,
						movement_input * m_MoveSpeed * t->Rotation(),
						Time::UnscaledDeltaTime() * 5.0
					);
					
					t->Position(t->Position() + (m_Motion * Time::UnscaledDeltaTime<scalar_t>()));
				}
			}
			
			// Cool slow motion effect when 'P' is held:
			{
				const tick_t targetScale = Input::Input::Key::Get(SDL_SCANCODE_P) ? 0.0 : 1.0;
				const tick_t effectSpeed = 3.0;
				
				Time::Scale(glm::mix(Time::Scale(), targetScale, Time::UnscaledDeltaTime() * effectSpeed));
			}
		}}
	}
	
	void FlyCam::SyncCameraSettings() {
		
		if (const auto c = m_Camera.lock()) {
			
			c->FOV     (Settings::Graphics::Perspective::s_FOV);
			c->NearClip(Settings::Graphics::Perspective::s_NearClip);
			c->FarClip (Settings::Graphics::Perspective::s_FarClip);
		}
	}
	
} // LouiEriksson::Game::Scripts