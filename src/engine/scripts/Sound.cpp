#include "stdafx.h"

#include "Sound.h"

void Sound::Init() {
	SDL_InitSubSystem(SDL_INIT_AUDIO);
}

void Sound::Play(const Sound::Clip& _clip) {

	for (int i = 0; i < SDL_GetNumAudioDevices((int)false); i++) {
		std::cout << SDL_GetAudioDeviceName(i, (int)false) << "\n";
	}
	
	auto device = SDL_OpenAudioDevice("Crusher ANC", 0, &_clip.m_Specification, nullptr, 0);
	
	SDL_QueueAudio(device, _clip.data, _clip.size);
	SDL_PauseAudioDevice(device, 0);
	
	//SDL_Delay(30000);
	
	SDL_CloseAudioDevice(device);
}

Sound::Clip::Clip(const std::filesystem::path& _path) {
	
	// Load the audio data using SDL.
	SDL_LoadWAV(_path.c_str(), &m_Specification, &data, &size);
}

Sound::Clip::~Clip() {
	SDL_FreeWAV(data);
}