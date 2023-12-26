#include "stdafx.h"

#include "Sound.h"

void Sound::Init() {
	SDL_InitSubSystem(SDL_INIT_AUDIO);
}

void Sound::Play(const Sound::Clip& _clip) {

	auto device = SDL_OpenAudioDevice(nullptr, 0, &_clip.m_Specification, nullptr, 0);
	
	SDL_QueueAudio(device, _clip.m_Buffer.data(), _clip.m_Buffer.size());
	
	SDL_CloseAudioDevice(device);
	SDL_FreeWAV((Uint8*)_clip.m_Buffer.data());
}

Sound::Clip::Clip(const std::string& _path) {
	
	// Load the audio data into a c-style byte array using SDL.
	Uint8* bytes;
	Uint32 size;
	
	SDL_LoadWAV(_path.c_str(), &m_Specification, &bytes, &size);
	
	// Insert the data from the c-style array into an std::vector for safety.
	m_Buffer.reserve(size);
	std::copy(bytes, bytes + size, m_Buffer.begin());
	
}