#ifndef GAME_FLYCAM_HPP
#define GAME_FLYCAM_HPP

#include "../include/engine_audio.hpp"
#include "../include/engine_core.hpp"
#include "../include/engine_graphics.hpp"

using namespace LouiEriksson::Engine;

namespace LouiEriksson::Game::Scripts {
	
	/**
	 * \class FlyCam
	 *
	 * \brief Free-fly camera.
	 *
	 * FlyCam is a class that represents a free-fly camera.
	 * It inherits from the Script class and provides functionality for controlling the camera's position and rotation.
	 */
	class FlyCam final : public Script {
	
	public:
	
		explicit FlyCam(const std::weak_ptr<ECS::GameObject>& _parent) : Script(_parent),
			m_Motion   ( 0.0),
			m_MoveSpeed( 5.0),
			m_LookSpeed(30.0),
			m_Rotation ( 0.0) {}
		
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(FlyCam); };
		
	protected:
	
		/** @brief Camera of the FlyCam. */
		std::weak_ptr<Graphics::Camera> m_Camera;
	
		/** @brief Transform of the FlyCam. */
		std::weak_ptr<Transform> m_Transform;
		
		/** @brief AudioListener component. */
		std::weak_ptr<Audio::AudioListener> m_AudioListener;
		
		/** @brief AudioSource component for gun sounds. */
		std::weak_ptr<Audio::AudioSource> m_GunSound;
		
		/** @brief Motion vector of the FlyCam. */
		glm::vec3 m_Motion;
		
		/** @brief Speed of FlyCam along each axis. */
		float m_MoveSpeed;
		
		/** @brief Speed of FlyCam rotation. */
		float m_LookSpeed;
		
		/** @brief Current rotation of the flycam in degrees. */
		glm::vec3 m_Rotation;
		
		/** @inheritdoc */
		void Begin() override {
		
			if (const auto p = Parent()) {
			if (const auto s = p->GetScene().lock()) {
			
				// Add a light to the scene for testing purposes.
				// TODO: Add a light to the scene through the scene's file, not code.
				{
					const auto light_gameObject = s->Create("Light");
					light_gameObject->AddComponent<Transform>();
					light_gameObject->AddComponent<Graphics::Light>();
				}
				
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
					
					c->SetTransform(t);
					c->ClearColor(glm::vec4(0.0));
					
					if (const auto w = Window::Get(2).lock()) {
						w->Attach(c);
					}
				}}
			}}
		}
	
		/** @inheritdoc */
		void Tick() override {
			
			// Update the camera's parameters to match the ones in Settings.
			SyncCameraSettings();
			
			if (const auto c = m_Camera.lock()) {
			if (const auto t = m_Transform.lock()) {
			
				// Lock the cursor to the center of the screen if the window is in focus:
				if (const auto v = c->GetViewport().lock()) {
					
					auto w = std::dynamic_pointer_cast<Window>(v);
					
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
					m_Rotation = Utils::WrapAngle(m_Rotation + (mouse_input * m_LookSpeed));
					m_Rotation.x = std::clamp(m_Rotation.x, static_cast<scalar_t>(-89.999), static_cast<scalar_t>(89.999));
					
					const auto direction = glm::vec3(
						glm::cos(glm::radians(m_Rotation.y)) * glm::cos(glm::radians(m_Rotation.x)),
						glm::sin(glm::radians(m_Rotation.x)),
						glm::sin(glm::radians(m_Rotation.y)) * glm::cos(glm::radians(m_Rotation.x))
					);
					
					t->Rotation(glm::quat(glm::lookAt(VEC_ZERO, direction, VEC_UP)));
					
					// Move the player:
					m_Motion = glm::mix(
						m_Motion,
						movement_input * m_MoveSpeed * t->Rotation(),
						Time::UnscaledDeltaTime() * 5.0
					);
					
					t->Position(t->Position() + (m_Motion * Time::UnscaledDeltaTime<scalar_t>()));
				}
				
				// Cool slow motion effect when 'P' is held:
				const tick_t targetScale = Input::Input::Key::Get(SDL_SCANCODE_P) ? 0.0 : 1.0;
				const tick_t effectSpeed = 3.0;
				
				Time::Scale(glm::mix(Time::Scale(), targetScale, Time::UnscaledDeltaTime() * effectSpeed));
			}}
		}
		
		/** @brief Synchronise the camera's parameters with the ones defined in Settings. */
		void SyncCameraSettings() {
			
			if (const auto c = m_Camera.lock()) {
				
				c->FOV     (Settings::Graphics::Perspective::s_FOV);
				c->NearClip(Settings::Graphics::Perspective::s_NearClip);
				c->FarClip (Settings::Graphics::Perspective::s_FarClip);
			}
		}
	
	};
	
} // LouiEriksson::Game::Scripts

#endif //GAME_FLYCAM_HPP