#ifndef FINALYEARPROJECT_AUDIOLISTENER_HPP
#define FINALYEARPROJECT_AUDIOLISTENER_HPP

#include "../core/Script.hpp"
#include "../core/Time.hpp"
#include "../core/Transform.hpp"
#include "../core/Types.hpp"
#include "../ecs/Component.hpp"
#include "../physics/Rigidbody.hpp"

#include <al.h>

#include <memory>
#include <typeindex>

namespace LouiEriksson::Engine::ECS {
	
	class GameObject;
	
} // LouiEriksson::Engine::ECS

namespace LouiEriksson::Engine::Audio {
	
	/**
	 * @class AudioListener
	 * @brief Represents an audio listener in the scene.
	 *
	 * The AudioListener class is responsible for synchronizing the audio listener with the internal audio engine.
	 * It provides functionalities to set and value the master gain of the audio listener. The position, orientation,
	 * and velocity of the audio listener are automatically updated based on the parent Parent's transform
	 * and rigidbody (if available).
	 *
	 * @see Please kindly refer to the <a href="https://github.com/kcat/openal-soft/wiki/Programmer%27s-Guide">OpenAL-Soft spec</a> for more information on OpenAL.
	 */
	class AudioListener final : public Script {
	
	private:
		
		struct Parameters {
			
			/** @brief Master gain. Should be between zero and one. */
			ALfloat m_Gain = 0.8;
			
			/** @brief The position of the AudioListener. */
			glm::vec3 m_Position;
			
			/** @brief The forward direction of the AudioListener. */
			glm::vec3 m_Forward;
			
			/** @brief The upward direction of the AudioListener. */
			glm::vec3 m_Up;
			
			/** @brief The velocity of the AudioListener. */
			glm::vec3 m_Velocity;
		};
		
		Parameters m_CurrentParameters;
		Parameters m_PendingParameters;
		
		/** @brief Synchronise the AudioListener with the internal audio engine. */
		void Sync() {
		
			if (const auto p = Parent()) {
			if (const auto t = p->GetComponent<Transform>()) {
				
				m_PendingParameters.m_Position = t->Position();
				m_PendingParameters.m_Forward  = t->FORWARD;
				m_PendingParameters.m_Up       = t->UP;
				
				// Set position of listener:
				if (m_PendingParameters.m_Position != m_CurrentParameters.m_Position) {
					alListenerfv(AL_POSITION, static_cast<const ALfloat*>(&(m_PendingParameters.m_Position[0])));
				}
				
				// Set orientation (forward, up):
				if (m_PendingParameters.m_Forward != m_CurrentParameters.m_Forward ||
					m_PendingParameters.m_Up      != m_CurrentParameters.m_Up
				){
					std::array<ALfloat, 6U> orientation {
						m_PendingParameters.m_Forward.x,
						m_PendingParameters.m_Forward.y,
						m_PendingParameters.m_Forward.z,
						m_PendingParameters.m_Up.x,
						m_PendingParameters.m_Up.y,
						m_PendingParameters.m_Up.z
					};
					
					alListenerfv(AL_ORIENTATION, orientation.data());
				}
				
				// Set velocity (using rigidbody, if available):
				{
					glm::vec3 velocity;
					
					if (const auto r = p->GetComponent<Physics::Rigidbody>()) {
						velocity = r->Velocity();
					}
					else {
						velocity = (t->Position() - m_CurrentParameters.m_Position) * Time::DeltaTime<scalar_t>();
					}
					
					alListenerfv(AL_VELOCITY, static_cast<const ALfloat*>(&velocity[0]));
				}
				
				// Assign gain value:
				if (m_PendingParameters.m_Gain != m_CurrentParameters.m_Gain) {
					alListenerf(AL_GAIN, m_PendingParameters.m_Gain);
				}
				
				m_PendingParameters = m_CurrentParameters;
			}}
		}
		
	public:
		
		explicit AudioListener(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : Script(_parent),
			m_CurrentParameters(),
			m_PendingParameters() {}
		
		/** @inheritdoc */
		[[nodiscard]] virtual std::type_index TypeID() const noexcept override { return typeid(AudioListener); };
		
		/** @brief Initialise the AudioListener. */
		virtual void Begin() override {
			Sync();
		}
	
		/** @brief Updates the AudioListener every frame. */
		virtual void Tick() override {
			Sync();
		}
		
		/**
		 * @brief Set the master gain of the AudioListener.
		 *
		 * This function sets the master gain of the AudioListener, which determines the volume of all audio played by the listener.
		 * @note: The _value parameter should be between 0.0 and 1.0, with 0.0 being mute and 1.0 being full volume.
		 *
		 * @param[in] _value The value of the master gain to set.
		 */
		void Gain(const ALfloat& _value) noexcept {
			m_PendingParameters.m_Gain = std::clamp(_value, static_cast<ALfloat>(0.0), static_cast<ALfloat>(1.0));
		}
		
		/**
		 * @brief Get the master gain of the AudioListener.
		 *
		 * This function returns the master gain of the AudioListener.
		 *
		 * @return The master gain value.
		 */
		[[nodiscard]] constexpr const ALfloat& Gain() const noexcept {
			return m_PendingParameters.m_Gain;
		}
		
	};
	
} // LouiEriksson::Engine::Audio

#endif //FINALYEARPROJECT_AUDIOLISTENER_HPP