#include "stdafx.h"

#include "Sound.h"

Sound::Clip::Format::Format(const SDL_AudioSpec& _audioSpec) {
	m_Specification = _audioSpec;
}

ALenum Sound::Clip::Format::OpenALFormat() const {
	
	ALenum result = AL_NONE;
	
	if (m_Specification.channels == 1) {
		
		switch (m_Specification.format) {
	        case AUDIO_U8:
	        case AUDIO_S8:     { result = AL_FORMAT_MONO8;  break; }
	        case AUDIO_U16LSB:
	        case AUDIO_S16LSB:
	        case AUDIO_U16MSB:
	        case AUDIO_S16MSB: { result = AL_FORMAT_MONO16; break; }
	        default: {
				std::cout << "Unimplemented format.\n";
			}
		}
	}
	else {
		switch (m_Specification.format) {
	        case AUDIO_U8:
	        case AUDIO_S8:     { result = AL_FORMAT_STEREO8;  break; }
	        case AUDIO_U16LSB:
	        case AUDIO_S16LSB:
	        case AUDIO_U16MSB:
	        case AUDIO_S16MSB: { result = AL_FORMAT_STEREO16; break; }
	        default: {
				std::cout << "Unimplemented format.\n";
			}
		}
	}
	
	return result;
}

Sound::Clip::Clip(const std::filesystem::path& _path) : m_Format({}) {
	
	// Generate audio buffer.
	alGenBuffers(1, &m_ALBuffer);

	{
		SDL_AudioSpec spec;
		
		// Load the audio data into a c-style byte array using SDL.
		SDL_LoadWAV(_path.c_str(), &spec, &m_Data, &m_Size);
		
		m_Format = Format(spec);
	}
	
	// Buffer the audio data and free the loaded data.
	if (m_ALBuffer != AL_NONE) {

		alBufferData(
			m_ALBuffer,
			m_Format.OpenALFormat(),
			m_Data,
			static_cast<ALsizei>(m_Size),
			m_Format.m_Specification.freq
		);
	}
}

Sound::Clip::~Clip() {
	Dispose();
}

void Sound::Clip::Dispose() {
	
	Free();
	
	// Delete the buffer.
	if (m_ALBuffer != AL_NONE) { alDeleteBuffers(1, &m_ALBuffer); m_ALBuffer = AL_NONE; }
}

void Sound::Clip::Free() {
	SDL_FreeWAV(m_Data);
	
	m_Size = 0;
}

void Sound::Init() {
	
	std::cout << "Initialising audio subsystems...\n";
	
	try {
		
		// Init SDL audio subsystem (as fallback).
		try {
			
			std::cout << "\tSDL...";
			SDL_InitSubSystem(SDL_INIT_AUDIO);
			std::cout << "Done.\n";
		}
		catch (const std::exception& e) {
			std::cout << "Failed.\n";
			
			throw e;
		}
		
		// Init OpenAL audio subsystem.
		try {
			
			std::cout << "\tOpenAL...";
		
			// Initialise audio device.
			if (s_Device == nullptr) {
				s_Device = alcOpenDevice(nullptr);
				
				if (s_Device == nullptr) {
					throw std::runtime_error("Failed opening audio device!");
				}
			}
			
			// Create an audio context.
			if (s_Context == nullptr) {
		        s_Context = alcCreateContext(s_Device, nullptr);
				
				if (s_Context == nullptr) {
					throw std::runtime_error("Failed creating audio context!");
				}
			}
			
			// Set s_Context as the current audio context.
			if (alcMakeContextCurrent(s_Context) == AL_NONE) {
				throw std::runtime_error("Failed setting audio context!");
			}
			
			alGenSources(1, &s_GlobalSource);
			
			if (s_GlobalSource == AL_NONE) {
				throw std::runtime_error("Failed creating global audio source!");
			}
			
			std::cout << "Done.\n";
		}
		catch (const std::exception& e) {
			std::cout << "Failed.\n";
			
			throw e;
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << '\n';
	}
}

void Sound::PlayGlobal(const Sound::Clip& _clip) {

	if (_clip.m_Size > 0) {
	
		if (_clip.m_ALBuffer == AL_NONE) {
			
			std::cout << "Playing using SDL!\n";
			
			if (s_SDL_Device > 0u) { SDL_CloseAudioDevice(s_SDL_Device); s_SDL_Device = 0u; }
			
			s_SDL_Device = SDL_OpenAudioDevice(nullptr, 0, &_clip.m_Format.m_Specification, nullptr, 0);
			
			// Play using SDL!
			SDL_QueueAudio(s_SDL_Device, _clip.m_Data, _clip.m_Size);
			SDL_PauseAudioDevice(s_SDL_Device, 0);
		}
		else {
			
			std::cout << "Playing using OpenAL!\n";
			
			// Play using OpenAL!
			alSourcei(s_GlobalSource, AL_BUFFER, static_cast<ALint>(_clip.m_ALBuffer));
			alSourcePlay(s_GlobalSource);
		}
	}
}

void Sound::Dispose() {
	
	if (s_GlobalSource != AL_NONE) {
		alDeleteSources(1, &s_GlobalSource);
	}
	
    alcMakeContextCurrent(nullptr);
	
	if (s_Context != nullptr) { alcDestroyContext(s_Context); s_Context = nullptr; }
	if (s_Device  != nullptr) { alcCloseDevice(s_Device);     s_Device  = nullptr; }
	
	if (s_SDL_Device > 0u) { SDL_CloseAudioDevice(s_SDL_Device); s_SDL_Device = 0u; }
}