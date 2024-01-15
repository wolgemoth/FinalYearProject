#include "AudioListener.h"

#include "../core/Script.h"
#include "../core/Time.h"
#include "../core/Transform.h"
#include "../ecs/GameObject.h"
#include "../physics/Rigidbody.h"

#include <al.h>
#include <glm/common.hpp>

#include <memory>

namespace LouiEriksson::Audio {
	
	AudioListener::AudioListener(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : Script(_parent),
			m_Gain        (1.0f),
			m_LastPosition(0.0f) {}
	
	AudioListener::~AudioListener() = default;
	
	void AudioListener::Begin() {
		Sync();
	}
	
	void AudioListener::Tick() {
		Sync();
	}
	
	void AudioListener::Sync() {
	
		if (const auto p = Parent().lock()) {
		if (const auto t = p->GetComponent<Transform>().lock()) {
			
			// Set position of listener:
			alListenerfv(AL_POSITION, static_cast<ALfloat*>(&t->m_Position[0]));
			
			// Set orientation (forward, up):
			{
				const auto f = t->FORWARD;
				const auto u = t->UP;
				
				const ALfloat orientation[] = { f.x, f.y, f.z, u.x, u.y, u.z };
				alListenerfv(AL_ORIENTATION, orientation);
			}
			
			// Set velocity (using rigidbody, if available):
			{
				glm::vec3 velocity;
				
				if (const auto r = p->GetComponent<Physics::Rigidbody>().lock()) {
					velocity = r->Velocity();
				}
				else {
					velocity = (t->m_Position - m_LastPosition) * Time::DeltaTime();
				}
				
				alListenerfv(AL_VELOCITY, static_cast<ALfloat*>(&velocity[0]));
			}
			
			
			// Assign gain value:
			alListenerf(AL_GAIN, m_Gain);
			
			// Update "last position" (used for transform-based doppler effect).
			m_LastPosition = t->m_Position;
		}}
	}
	
	void AudioListener::Gain(const float& _value) noexcept {
		m_Gain = glm::clamp(_value, 0.0f, 1.0f);
	}
	const float& AudioListener::Gain() const noexcept {
		return m_Gain;
	}
	
} // LouiEriksson::Audio