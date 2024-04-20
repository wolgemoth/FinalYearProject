#include "AudioSource.h"

#include "../audio/AudioClip.h"
#include "../core/Debug.h"
#include "../core/Script.h"
#include "../core/Time.h"
#include "../core/Transform.h"
#include "../ecs/GameObject.h"
#include "../physics/Rigidbody.h"

#include "Sound.h"

#include <al.h>
#include <algorithm>
#include <glm/common.hpp>

#include <exception>
#include <limits>
#include <memory>
#include <stdexcept>

namespace LouiEriksson::Engine::Audio {
	
	AudioSource::Parameters::Parameters() noexcept :
			m_IsGlobal(false), // Do not start as global.
			m_Loop    (false), // Do not loop.
			
			// Default panning value. (Will only apply if if the audio source is global.)
			m_Panning( 0.0),
			 
			// Set default minimum and maximum distances.
			m_MinDistance(  1.0),
			m_MaxDistance(100.0),
			
			// Set other defaults:
			m_Pitch       (  1.0),
			m_GainModifier(  1.0),
			m_MinGain     (  0.0),
			m_MaxGain     (  1.0),
			m_Rolloff     (  1.0),
			m_MinAngle    (360.0),
			m_MaxAngle    (360.0) {}
	
	AudioSource::Parameters::~Parameters() = default;
	
	void AudioSource::Sync() {
		
		// TODO: Some sort of caching so these values aren't changed unnecessarily.
		
		// Set values that do not change depending on if the source is global or not:
		alSourcef(m_Source, AL_PITCH,          m_Parameters.m_Pitch);
		alSourcef(m_Source, AL_GAIN,           m_Parameters.m_GainModifier);
		alSourcef(m_Source, AL_MIN_GAIN,       m_Parameters.m_MinGain);
		alSourcef(m_Source, AL_MAX_GAIN,       m_Parameters.m_MaxGain);
		alSourcef(m_Source, AL_ROLLOFF_FACTOR, m_Parameters.m_Rolloff);
		
		// If the audio source is global...
		if (m_Parameters.m_IsGlobal) {
			
			/* NULLIFY EFFECT OF ORIENTATION */
	
			/*
			 * If AL_SOURCE_RELATIVE is true, the audio is played relative to the listener.
			 *
			 * In this case, the position of the source is (0.0, 0.0, 0.0) units
			 * from the listener's position (or directly on top of the listener).
			 *
			 * This will cause the panning of the audio to always be centered, regardless of
			 * the position or orientation of the audio listener.
			 */
			alSourcei(m_Source, AL_SOURCE_RELATIVE, AL_TRUE);
			
			/* SET PANNING */
			
			// Global AudioSources have relative position to the listener, so update local position only.
			alSourcefv(m_Source, AL_POSITION, static_cast<ALfloat*>(&m_Parameters.m_Panning[0]));
			
			/* DISABLE ATTENUATION */
			
			// Setting reference distance to 0.0 disables attenuation.
			alSourcef(m_Source, AL_REFERENCE_DISTANCE, 0.0);
			
			// Set max distance to infinity (not strictly-necessary).
			alSourcef(m_Source, AL_MAX_DISTANCE, std::numeric_limits<ALfloat>::max());
			
			// Nullify directionality of AudioSource
			alSourcef(m_Source, AL_CONE_OUTER_ANGLE, 360.0);
			alSourcef(m_Source, AL_CONE_OUTER_ANGLE, 360.0);
			
			/* RESET VELOCITY */
			alListener3f(AL_VELOCITY, 0.0, 0.0, 0.0);
		}
		else {
		
			// Set all defined parameters in a positional context:
			alSourcef(m_Source, AL_REFERENCE_DISTANCE, m_Parameters.m_MinDistance);
			alSourcef(m_Source, AL_MAX_DISTANCE,       m_Parameters.m_MaxDistance);
			
			// Set min and max cone angles of the source.
			alSourcef(m_Source, AL_CONE_OUTER_ANGLE, m_Parameters.m_MinAngle);
			alSourcef(m_Source, AL_CONE_OUTER_ANGLE, m_Parameters.m_MaxAngle);
			
			if (const auto p = Parent().lock()) {
			if (const auto t = p->GetComponent<Transform>().lock()) {
				
				// Set world position to that of the current transform.
				alSourcefv(m_Source, AL_POSITION, static_cast<const ALfloat*>(&(t->Position()[0])));
				
				// Set the direction of the audio source using the current transform.
				{
					const auto dir = t->FORWARD;
					
					alSource3f(m_Source, AL_DIRECTION, dir.x, dir.y, dir.z);
				}
			
				// Set velocity (using rigidbody, if available):
				{
					glm::vec3 velocity;
					
					if (const auto r = p->GetComponent<Physics::Rigidbody>().lock()) {
						velocity = r->Velocity();
					}
					else {
						velocity = (t->Position() - m_LastPosition) * Time::DeltaTime<scalar_t >();
					}
					
					alListenerfv(AL_VELOCITY, static_cast<ALfloat*>(&velocity[0]));
				}
				
				m_LastPosition = t->Position();
			}}
		}
	}
	
	AudioSource::AudioSource(const std::weak_ptr<ECS::GameObject>& _parent) : Script(_parent),
			m_Source      (AL_NONE),
			m_Parameters  (),
			m_LastPosition(0.0)
	{
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
			Debug::Log(e);
		}
	}
	
	AudioSource::~AudioSource() {
		
		if (m_Source != AL_NONE) {
			alDeleteSources(1, &m_Source);
		}
	}
	
	void AudioSource::Begin() {
		Sync();
	}
	
	void AudioSource::Tick() {
		Sync();
	}
	
	void AudioSource::Play(const bool& _allowFallback) {
		
		try {
			
			// Try and get current clip.
			if (const auto c = m_Clip.lock()) {
				
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
						
						// If already playing, nothing needs to be done.
						if (State() != AL_PLAYING) {
							
							// Play!
							alSourcei(m_Source, AL_BUFFER, static_cast<ALint>(c->m_ALBuffer));
							alSourcePlay(m_Source);
						}
						else {
							throw std::runtime_error("Attempted to call Play() on an AudioSource that is already playing.");
						}
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
			Debug::Log(e);
		}
	}
	
	void AudioSource::Pause() const {
		alSourcePause(m_Source);
	}
	
	void AudioSource::Stop() const {
		alSourceStop(m_Source);
	}
	
	void AudioSource::Clip(const std::weak_ptr<AudioClip>& _value) noexcept {
		m_Clip = _value;
	}
	const std::weak_ptr<AudioClip>& AudioSource::Clip() const noexcept {
		return m_Clip;
	}
	
	ALenum AudioSource::State() const {
		
		ALint state;
		
	    alGetSourcei(m_Source, AL_SOURCE_STATE, &state);
		
	    return static_cast<ALenum>(state);
	}
	
	void AudioSource::Global(const bool& _value) {
		m_Parameters.m_IsGlobal = _value;
		
		Sync();
	}
	const bool& AudioSource::Global() const noexcept {
		return m_Parameters.m_IsGlobal;
	}
	
	void AudioSource::Loop(const bool& _value) {
		m_Parameters.m_Loop = _value;
		
		Sync();
	}
	const bool& AudioSource::Loop() const noexcept {
		return m_Parameters.m_Loop;
	}
	
	void AudioSource::MinDistance(const ALfloat& _value) {
		m_Parameters.m_MinDistance = std::max(_value, std::numeric_limits<ALfloat>::min());
		m_Parameters.m_MaxDistance = std::max(_value, m_Parameters.m_MaxDistance);
		
		Sync();
	}
	const ALfloat& AudioSource::MinDistance() const noexcept {
		return m_Parameters.m_MinDistance;
	}
	
	void AudioSource::MaxDistance(const ALfloat& _value) {
		m_Parameters.m_MaxDistance = std::max(_value, m_Parameters.m_MinDistance);
		
		Sync();
	}
	const ALfloat& AudioSource::MaxDistance() const noexcept {
		return m_Parameters.m_MaxDistance;
	}
	
	void AudioSource::Pitch(const ALfloat& _value) {
		m_Parameters.m_Pitch = std::max(_value, static_cast<ALfloat>(0.0));
		
		Sync();
	}
	const ALfloat& AudioSource::Pitch() const noexcept {
		return m_Parameters.m_Pitch;
	}
	
	void AudioSource::Gain(const ALfloat& _value) {
	    m_Parameters.m_GainModifier = std::max(_value, static_cast<ALfloat>(0.0));
		
		Sync();
	}
	const ALfloat& AudioSource::Gain() const noexcept {
		return m_Parameters.m_GainModifier;
	}
	
	void AudioSource::MinGain(const ALfloat& _value) {
	    m_Parameters.m_MinGain = std::max(_value, static_cast<ALfloat>(0.0));
	    m_Parameters.m_MaxGain = std::max(_value, m_Parameters.m_MaxGain);
		
		Sync();
	}
	const ALfloat& AudioSource::MinGain() const noexcept {
		return m_Parameters.m_MinGain;
	}
	
	void AudioSource::MaxGain(const ALfloat& _value) {
	    m_Parameters.m_MaxGain = std::max(_value, m_Parameters.m_MinGain);
		
		Sync();
	}
	const ALfloat& AudioSource::MaxGain() const noexcept {
		return m_Parameters.m_MaxGain;
	}
	
	void AudioSource::Rolloff(const ALfloat& _value) {
	    m_Parameters.m_Rolloff = _value;
		
		Sync();
	}
	const ALfloat& AudioSource::Rolloff() const noexcept {
		return m_Parameters.m_Rolloff;
	}
	
	void AudioSource::MinAngle(const ALfloat& _value) {
	    m_Parameters.m_MinAngle = std::clamp(_value, static_cast<ALfloat>(0.0), static_cast<ALfloat>(360.0));
		m_Parameters.m_MaxAngle = std::max(_value, m_Parameters.m_MaxAngle);
		
		Sync();
	}
	const ALfloat& AudioSource::MinAngle() const noexcept {
		return m_Parameters.m_MinAngle;
	}
	
	void AudioSource::MaxAngle(const ALfloat& _value) {
	    m_Parameters.m_MaxAngle = std::max(_value, m_Parameters.m_MinAngle);
		
		Sync();
	}
	const ALfloat& AudioSource::MaxAngle() const noexcept {
		return m_Parameters.m_MaxAngle;
	}
	
	void AudioSource::PlaybackPosition(const ALfloat& _value) const {
		alSourcef(m_Source, AL_SEC_OFFSET, _value);
	}
	ALfloat AudioSource::PlaybackPosition() const {
		
		ALfloat result;
		
        alGetSourcef(m_Source, AL_SEC_OFFSET, &result);
		
		return result;
	}
	
	void AudioSource::PlaybackPosition(const ALenum& _param, const int& _value) const {
		alSourcei(m_Source, _param, _value);
	}
	int AudioSource::PlaybackPosition(const ALenum& _param) const {
		
		int result;
		
        alGetSourcei(m_Source, _param, &result);
		
		return result;
	}
	
} // LouiEriksson::Engine::Audio