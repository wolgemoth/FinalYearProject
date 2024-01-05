#include "AudioSource.h"

namespace LouiEriksson {
	
	AudioSource::AudioSource(const std::shared_ptr<GameObject>& _parent) : Component(_parent) {
		
		m_Source = AL_NONE;
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
					throw std::runtime_error("Failed creating global audio source!");
				}
			}
			else {
				throw std::runtime_error("Attempted to initialise an AudioSource which is already initialised!");
			}
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
		}
	}
	
	void AudioSource::Play(const Sound::Clip& _clip) const {
		
		if (_clip.m_Size > 0) {
		
			if (_clip.m_ALBuffer == AL_NONE) {
				
				// AL buffer non-existent. Implies AL failed to initialise correctly.
				// Fall back to playing sound globally instead...
				
				Sound::PlayGlobal(_clip);
			}
			else {
				
				std::cout << "Playing using OpenAL!\n";
				
				// Play using OpenAL!
				alSourcei(m_Source, AL_BUFFER, static_cast<ALint>(_clip.m_ALBuffer));
				alSourcePlay(m_Source);
			}
		}
	}
	
} // LouiEriksson