#include "stdafx.h"

#include "AudioSource.h"

#include "Sound.h"
#include "Rigidbody.h"
#include "Time.h"

#include "Utils.h"

namespace LouiEriksson {
	
	AudioSource::Parameters::Parameters() {
	
		m_IsGlobal = false; // Do not start as global.
		m_Loop     = false; // Do not loop.
		
		// Default panning value. (Will only apply if if the audio source is global.)
		m_Panning = glm::vec3(0.0f);
		
		// Set default minimum and maximum distances.
		m_MinDistance = 1.0f;
		m_MaxDistance = 100.0f;
		
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
			
			// Get reference to parent (may be null).
			const auto parent = Parent();
			
			if (parent != nullptr) {
				
				// Get reference to transform (may be null).
				const auto transform = parent->GetComponent<Transform>();
				
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
					
					const auto rigidbody = parent->GetComponent<Rigidbody>();
					
					if (rigidbody != nullptr) {
						velocity = rigidbody->Velocity();
					}
					else {
						velocity = (transform->m_Position - m_LastPosition) * Time::DeltaTime();
					}
					
					alListenerfv(AL_VELOCITY, static_cast<ALfloat*>(&velocity[0]));
				}
				
				m_LastPosition = transform->m_Position;
			}
		}
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
	
	void AudioSource::Play(const bool& _allowFallback) {
		
		try {
			
			// Try and get current clip.
			const auto c = m_Clip.lock();
			
			if (c != nullptr) {
				
				// Only play if the clip actually contains data.
				if (c->m_Samples.m_Length > 0) {
				
					if (c->m_ALBuffer == AL_NONE) {
						
						/*
						 * AL buffer non-existent. Implies AL failed to initialise correctly.
						 * Attempt to play the clip globally instead. This will use whatever fallback is available.
						 */
						
						if (_allowFallback) {
							Sound::PlayGlobal(c);
						}
						else {
							throw std::runtime_error(
								"Cannot play audio clip since m_ALBuffer == AL_NONE, "
								"and permission to use a fallback is denied!\n"
							);
						}
					}
					else {
						
						// Play!
						alSourcei(m_Source, AL_BUFFER, static_cast<ALint>(c->m_ALBuffer));
						alSourcePlay(m_Source);
					}
				}
				else {
					throw std::runtime_error("Cannot play AudioClip since its size is zero!");
				}
			}
			else {
				throw std::runtime_error("Cannot play AudioSource since the current clip is nullptr!");
			}
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
		}
	}
	
	void AudioSource::Pause() {
		alSourcePause(m_Source);
	}
	
	void AudioSource::Stop() {
		alSourceStop(m_Source);
	}
	
	void AudioSource::Clip(const std::weak_ptr<AudioClip>& _value) {
		m_Clip = _value;
	}
	const std::weak_ptr<AudioClip>& AudioSource::Clip() const {
		return m_Clip;
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
	
	void AudioSource::PlaybackPosition(const float& _value) {
		alSourcef(m_Source, AL_SEC_OFFSET, _value);
	}
	float AudioSource::PlaybackPosition() const {
		
		float result;
		
        alGetSourcef(m_Source, AL_SEC_OFFSET, &result);
		
		return result;
	}
	
	void AudioSource::PlaybackPosition(const ALenum& _param, const int& _value) {
		alSourcei(m_Source, _param, _value);
	}
	int AudioSource::PlaybackPosition(const ALenum& _param) const {
		
		int result;
		
        alGetSourcei(m_Source, _param, &result);
		
		return result;
	}
	
} // LouiEriksson