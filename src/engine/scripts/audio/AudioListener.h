#ifndef FINALYEARPROJECT_AUDIOLISTENER_H
#define FINALYEARPROJECT_AUDIOLISTENER_H

#include "../core/Script.h"
#include "../core/Time.h"
#include "../core/Transform.h"
#include "../core/Types.h"
#include "../ecs/Component.h"
#include "../ecs/GameObject.h"
#include "../physics/Rigidbody.h"

#include <al.h>

#include <glm/common.hpp>

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
	 * and velocity of the audio listener are automatically updated based on the parent GameObject's transform
	 * and rigidbody (if available).
	 *
	 * @see Please kindly refer to the <a href="https://github.com/kcat/openal-soft/wiki/Programmer%27s-Guide">OpenAL-Soft spec</a> for more information on OpenAL.
	 */
	class AudioListener final : public Script {
	
	private:
		
		/** @brief Master gain. Should be between zero and one. */
		float m_Gain;
		
		/** @brief Last position of the AudioListener (for computing transform-based velocity). */
		glm::vec3 m_LastPosition;
		
		/** @brief Synchronise the AudioListener with the internal audio engine. */
		void Sync() {
		
			if (const auto p = Parent().lock()) {
			if (const auto t = p->GetComponent<Transform>().lock()) {
				
				// Set position of listener:
				alListenerfv(AL_POSITION, static_cast<const ALfloat*>(&(t->Position()[0])));
				
				// Set orientation (forward, up):
				{
					const auto f = t->FORWARD;
					const auto u = t->UP;
					
					const ALfloat orientation[] { f.x, f.y, f.z, u.x, u.y, u.z };
					alListenerfv(AL_ORIENTATION, orientation);
				}
				
				// Set velocity (using rigidbody, if available):
				{
					glm::vec3 velocity;
					
					if (const auto r = p->GetComponent<Physics::Rigidbody>().lock()) {
						velocity = r->Velocity();
					}
					else {
						velocity = (t->Position() - m_LastPosition) * Time::DeltaTime<scalar_t>();
					}
					
					alListenerfv(AL_VELOCITY, static_cast<const ALfloat*>(&velocity[0]));
				}
				
				
				// Assign gain value:
				alListenerf(AL_GAIN, m_Gain);
				
				// Update "last position" (used for transform-based doppler effect).
				m_LastPosition = t->Position();
			}}
		}
		
	public:
		
		explicit AudioListener(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : Script(_parent),
			m_Gain        (1.0),
			m_LastPosition(0.0) {}
		
		/** @inheritdoc */
		[[nodiscard]] inline std::type_index TypeID() const noexcept override { return typeid(AudioListener); };
		
		/** @brief Initialise the AudioListener. */
		inline void Begin() override {
			Sync();
		}
	
		/** @brief Updates the AudioListener every frame. */
		inline void Tick() override {
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
		inline void Gain(const ALfloat& _value) noexcept {
			m_Gain = std::clamp(_value, static_cast<ALfloat>(0.0), static_cast<ALfloat>(1.0));
		}
		
		/**
		 * @brief Get the master gain of the AudioListener.
		 *
		 * This function returns the master gain of the AudioListener.
		 *
		 * @return The master gain value.
		 */
		[[nodiscard]] constexpr const ALfloat& Gain() const noexcept {
			return m_Gain;
		}
		
	};
	
} // LouiEriksson::Engine::Audio

#endif //FINALYEARPROJECT_AUDIOLISTENER_H