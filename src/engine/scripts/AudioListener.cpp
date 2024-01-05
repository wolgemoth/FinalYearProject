#include "AudioListener.h"

namespace LouiEriksson {
	
	AudioListener::AudioListener(const std::shared_ptr<GameObject>& _parent) : Component(_parent) {}
	AudioListener::~AudioListener() {}
	
	void AudioListener::Init() {
		Sync();
	}
	
	void AudioListener::Tick() {
		Sync();
	}
	
	void AudioListener::Sync() {
	
		const auto transform = Parent()->GetComponent<Transform>();
		
		if (transform != nullptr) {
			
			// Set position:
			alListenerfv(AL_POSITION, static_cast<ALfloat*>(&transform->m_Position[0]));
			
			// Set orientation (forward, up):
			const auto f = transform->FORWARD;
			const auto u = transform->UP;
			
			const ALfloat orientation[] = { f.x, f.y, f.z, u.x, u.y, u.z };
			alListenerfv(AL_ORIENTATION, orientation);
		}
	}
	
} // LouiEriksson