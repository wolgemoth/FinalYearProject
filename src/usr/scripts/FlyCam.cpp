#include "FlyCam.h"

#include "../../engine/scripts/audio/AudioSource.h"
#include "../../engine/scripts/audio/AudioListener.h"
#include "../../engine/scripts/core/Resources.h"
#include "../../engine/scripts/core/Script.h"
#include "../../engine/scripts/core/Settings.h"
#include "../../engine/scripts/core/Time.h"
#include "../../engine/scripts/core/Transform.h"
#include "../../engine/scripts/core/utils/Utils.h"
#include "../../engine/scripts/core/Window.h"
#include "../../engine/scripts/ecs/GameObject.h"
#include "../../engine/scripts/ecs/Scene.h"
#include "../../engine/scripts/graphics/Camera.h"
#include "../../engine/scripts/graphics/Light.h"
#include "../../engine/scripts/input/Cursor.h"
#include "../../engine/scripts/input/Input.h"

#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/vector_float4.hpp>

#include <SDL_mouse.h>
#include <SDL_scancode.h>

#include <memory>
#include <string>

// @Assessor: Please mark this class for GEP.

namespace LouiEriksson::Game {
	
	FlyCam::FlyCam(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : Script(_parent),
			m_Motion(0.0f),
			m_Rotation(0.0f),
			m_MoveSpeed(5.0f),
			m_LookSpeed(30.0f) {}
	
	FlyCam::~FlyCam() = default;
	
	void FlyCam::Begin() {
		
		if (const auto p =      Parent().lock()) {
		if (const auto s = p->GetScene().lock()) {
		
			// Get Transform.
			m_Transform = p->GetComponent<Transform>();
			
			// Get or add Camera.
			m_Camera = p->AddComponent<Graphics::Camera>().lock();
			
			// Add AudioListener.
			// TODO: Do this better:
			m_AudioListener = p->AddComponent<Audio::AudioListener>();
			if (const auto al = m_AudioListener.lock()) {
				al->Init();
			}
			
			// Add AudioSource.
			m_GunSound = p->AddComponent<Audio::AudioSource>();
			if (const auto as = m_GunSound.lock()) {
				as->Clip(Resources::GetAudio("machineGun").lock());
			}
			
			// Update the camera's parameters to match the ones in Settings.
			SyncCameraSettings();
			
			// Set camera defaults:
			if (const auto c =    m_Camera.lock()) {
			if (const auto t = m_Transform.lock()) {
				
				c->SetWindow(Window::Get(2));
				c->SetTransform(t);
				c->ClearColor(glm::vec4(0.0f));
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
		
		m_GunSound.lock()->Tick();
		
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
						 0.0f
					);
					
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
					   mouse_input = glm::vec3(0.0f);
					movement_input = glm::vec3(0.0f);
				}
				
				// Rotate the camera:
				{
					m_Rotation = Utils::WrapAngle(m_Rotation + (mouse_input * Time::DeltaTime() * m_LookSpeed));
					m_Rotation.x = glm::clamp(m_Rotation.x, -89.999f, 89.999f);
					
					const auto direction = glm::vec3(
						glm::cos(glm::radians(m_Rotation.y)) * glm::cos(glm::radians(m_Rotation.x)),
						glm::sin(glm::radians(m_Rotation.x)),
						glm::sin(glm::radians(m_Rotation.y)) * glm::cos(glm::radians(m_Rotation.x))
					);
					
					t->m_Rotation = glm::quat(glm::lookAt(VEC_ZERO, direction, VEC_UP));
				}
				
				// Move the player:
				{
					m_Motion = glm::mix(
						m_Motion,
						movement_input * m_MoveSpeed * t->m_Rotation,
						Time::DeltaTime() * 5.0f
					);
					
					t->m_Position += (m_Motion * Time::DeltaTime());
				}
			}
			
			// Cool slow motion effect when 'P' is held:
			{
				const auto targetScale = Input::Input::Key::Get(SDL_SCANCODE_P) ? 0.0f : 1.0f;
				const auto effectSpeed = 3.0f;
				
				Time::Scale(glm::mix(Time::Scale(), targetScale, Time::UnscaledDeltaTime() * effectSpeed));
			}
			
			m_AudioListener.lock()->Tick();
			m_GunSound.lock()->Tick();
		}}
	}
	
	void FlyCam::SyncCameraSettings() noexcept {
		
		if (const auto c = m_Camera.lock()) {
			
			c->FOV     (Settings::Graphics::Perspective::s_FOV);
			c->NearClip(Settings::Graphics::Perspective::s_NearClip);
			c->FarClip (Settings::Graphics::Perspective::s_FarClip);
		}
	}
	
} // LouiEriksson::Game