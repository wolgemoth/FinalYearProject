#include "stdafx.h"

#include "AudioSource.h"

#include "Rigidbody.h"
#include "Time.h"

namespace LouiEriksson {
	
	AudioSource::Parameters::Parameters() {
	
		m_IsGlobal = false; // Do not start global.
		m_Loop     = false; // Do not loop.
		
		// Default panning value. (Will only apply if if the audio source is global.)
		m_Panning = glm::vec3(0.0f);
		
		// Set default minimum and maximum distances.
		// Minimum should be smallest positive value as a value of 0.0 disables attenuation.
		m_MinDistance = std::numeric_limits<float>().epsilon();
		m_MaxDistance = 50.0f;
		
		// Set other defaults:
		m_Pitch        = 1.0f;
		m_GainModifier = 1.0f;
		m_MinGain      = 0.0f;
		m_MaxGain      = 1.0f;
		m_Rolloff      = 1.0f;
		m_MinAngle     = 360.0f;
		m_MaxAngle     = 360.0f;
	}
	
	AudioSource::Parameters::~Parameters() {}
	
	void AudioSource::Sync() {
	
		// Get reference to transform (can be null).
		const auto transform = Parent()->GetComponent<Transform>();
		
		// If the audio source is global...
		if (m_Parameters.m_IsGlobal) {
			
			/* NULLIFY EFFECT OF ORIENTATION */
	
			/*
			 * If AL_SOURCE_RELATIVE is true, the audio is played relative to the listener.
			 *
			 * In this case, the position of the source is (0.0f, 0.0f, 0.0f) units
			 * from the listener's position (or directly on top of the listener).
			 *
			 * This will cause the panning of the audio to always be centered, regardless of
			 * the position or orientation of the audio listener.
			 */
			alSourcei(m_Source, AL_SOURCE_RELATIVE, AL_TRUE);
			
			/* SET PANNING */
			
			// Global audio source has a relative position to the listener,
			// so update its local position only.
			alSourcefv(m_Source, AL_POSITION, static_cast<ALfloat*>(&m_Parameters.m_Panning[0]));
			
			/* DISABLE ATTENUATION */
			
			// Setting reference distance to 0.0 disables attenuation.
			alSourcef(m_Source, AL_REFERENCE_DISTANCE, 0.0f);
			
			// Set max distance to infinity (not strictly-necessary).
			alSourcef(m_Source, AL_MAX_DISTANCE, std::numeric_limits<float>::max());
			
			/* RESET VELOCITY */
			alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
		}
		else {
		
			// Set all defined parameters in a positional context:
			alSourcef(m_Source, AL_REFERENCE_DISTANCE, m_Parameters.m_MinDistance);
			alSourcef(m_Source, AL_MAX_DISTANCE,       m_Parameters.m_MaxDistance);
			
			if (transform != nullptr) {
				
				// Set world position to that of the current transform.
				alSourcefv(m_Source, AL_POSITION, static_cast<ALfloat*>(&transform->m_Position[0]));
				
				// Set the direction of the audio source using the current transform.
				{
					const auto dir = transform->FORWARD;
					
					alSource3f(m_Source, AL_DIRECTION, dir.x, dir.y, dir.z);
				}
			}
			
			// Set velocity (using rigidbody, if available):
			{
				glm::vec3 velocity;
				
				const auto rigidbody = Parent()->GetComponent<Rigidbody>();
				
				if (rigidbody != nullptr) {
					velocity = rigidbody->Velocity();
				}
				else {
					velocity = (transform->m_Position - m_LastPosition) * Time::DeltaTime();
				}
				
				alListenerfv(AL_VELOCITY, static_cast<ALfloat*>(&velocity[0]));
			}
		}
		
		m_LastPosition = transform->m_Position;
	}
	
	AudioSource::AudioSource(const std::shared_ptr<GameObject>& _parent) : Component(_parent) {
		
		m_Source = AL_NONE;
		
		m_Parameters = {};
	}
	
	AudioSource::~AudioSource() {
		
		if (m_Source != AL_NONE) {
			alDeleteSources(1, &m_Source);
		}
	}
	
	void AudioSource::Init() {
		
		try {
			
			if (m_Source == AL_NONE) {
				
				alGenSources(1, &m_Source);
				
				if (m_Source == AL_NONE) {
					throw std::runtime_error("Failed creating audio source!");
				}
				
				Sync();
			}
			else {
				throw std::runtime_error("Attempted to initialise an AudioSource which is already initialised!");
			}
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
		}
	}
	
	void AudioSource::Tick() {
		Sync();
	}
	
	void AudioSource::Play(const std::weak_ptr<Sound::Clip>& _clip) {
		
		// Set _clip as most recent clip.
		m_Clip = _clip;
		
		// Attempt to lock clip pointer and evaluate if it is valid.
		auto c = _clip.lock();
		
		if (c != nullptr) {
			
			// Only play if the clip actually contains data.
			if (c->m_Size > 0) {
			
				if (c->m_ALBuffer == AL_NONE) {
					
					/*
					 * AL buffer non-existent. Implies AL failed to initialise correctly.
					 * Attempt to play the clip globally instead. This will use whatever fallback is available.
					 */
					Sound::PlayGlobal(_clip);
				}
				else {
					
					// Play!
					alSourcei(m_Source, AL_BUFFER, static_cast<ALint>(c->m_ALBuffer));
					alSourcePlay(m_Source);
				}
			}
		}
	}
	
	void AudioSource::Global(const bool& _value) {
		m_Parameters.m_IsGlobal = _value;
		
		Sync();
	}
	const bool& AudioSource::Global() const {
		return m_Parameters.m_IsGlobal;
	}
	
	void AudioSource::Loop(const bool& _value) {
		m_Parameters.m_Loop = _value;
		
		Sync();
	}
	const bool& AudioSource::Loop() const {
		return m_Parameters.m_Loop;
	}
	
	void AudioSource::MinDistance(const float& _value) {
		m_Parameters.m_MinDistance = glm::max(_value, std::numeric_limits<float>().epsilon());
		m_Parameters.m_MaxDistance = glm::max(_value, m_Parameters.m_MaxDistance);
		
		Sync();
	}
	const float& AudioSource::MinDistance() const {
		return m_Parameters.m_MinDistance;
	}
	
	void AudioSource::MaxDistance(const float& _value) {
		m_Parameters.m_MaxDistance = glm::max(_value, m_Parameters.m_MinDistance);
		
		Sync();
	}
	const float& AudioSource::MaxDistance() const {
		return m_Parameters.m_MaxDistance;
	}
	
	void AudioSource::Pitch(const float& _value) {
		m_Parameters.m_Pitch = glm::max(_value, 0.0f);
		
		Sync();
	}
	const float& AudioSource::Pitch() const {
		return m_Parameters.m_Pitch;
	}
	
	void AudioSource::Gain(const float& _value) {
	    m_Parameters.m_GainModifier = glm::max(_value, 0.0f);
		
		Sync();
	}
	const float& AudioSource::Gain() const {
		return m_Parameters.m_GainModifier;
	}
	
	void AudioSource::MinGain(const float& _value) {
	    m_Parameters.m_MinGain = glm::max(_value, 0.0f);
	    m_Parameters.m_MaxGain = glm::max(_value, m_Parameters.m_MaxGain);
		
		Sync();
	}
	const float& AudioSource::MinGain() const {
		return m_Parameters.m_MinGain;
	}
	
	void AudioSource::MaxGain(const float& _value) {
	    m_Parameters.m_MaxGain = glm::max(_value, m_Parameters.m_MinGain);
		
		Sync();
	}
	const float& AudioSource::MaxGain() const {
		return m_Parameters.m_MaxGain;
	}
	
	void AudioSource::Rolloff(const float& _value) {
	    m_Parameters.m_Rolloff = _value;
		
		Sync();
	}
	const float& AudioSource::Rolloff() const {
		return m_Parameters.m_Rolloff;
	}
	
	void AudioSource::MinAngle(const float& _value) {
	    m_Parameters.m_MinAngle = glm::clamp(_value, 0.0f, 360.0f);
		m_Parameters.m_MaxAngle = glm::max(_value, m_Parameters.m_MaxAngle);
		
		Sync();
	}
	const float& AudioSource::MinAngle() const {
		return m_Parameters.m_MinAngle;
	}
	
	void AudioSource::MaxAngle(const float& _value) {
	    m_Parameters.m_MaxAngle = glm::max(_value, m_Parameters.m_MinAngle);
		
		Sync();
	}
	const float& AudioSource::MaxAngle() const {
		return m_Parameters.m_MaxAngle;
	}
	
} // LouiEriksson