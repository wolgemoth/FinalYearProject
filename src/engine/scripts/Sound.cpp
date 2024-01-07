#include "stdafx.h"

#include "Sound.h"

namespace LouiEriksson {
	
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
				
				// Set the default distance model for the audio context:
				DistanceModel(AL_INVERSE_DISTANCE);
				
				/*
				 * Generate the global audio source.
				 * This will be responsible for playing non-positional sound on-demand.
				 */
				s_GlobalSource.reset(new AudioSource(nullptr));
				s_GlobalSource->Init();
				
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
	
	void Sound::DistanceModel(const ALenum& _value) {
		alDistanceModel(_value);
	}
	
	void Sound::PlayGlobal(const std::weak_ptr<AudioClip>& _clip) {
	
		// Attempt to lock clip pointer and evaluate if it is valid.
		auto c = _clip.lock();
		
		if (c != nullptr) {
			
			// Only play if the clip actually contains data.
			if (c->m_Size > 0) {
			
				if (c->m_ALBuffer == AL_NONE) {
					
					if (s_SDL_Device > 0u) { SDL_CloseAudioDevice(s_SDL_Device); s_SDL_Device = 0u; }
					
					s_SDL_Device = SDL_OpenAudioDevice(nullptr, 0, &c->m_Format.m_Specification, nullptr, 0);
					
					// Play using SDL!
					SDL_QueueAudio(s_SDL_Device, c->m_Data, c->m_Size);
					SDL_PauseAudioDevice(s_SDL_Device, 0);
				}
				else {
					
					// Play the clip (without fallback, as that could potentially lead to infinite recursion).
					s_GlobalSource->Play(_clip, false);
				}
			}
		}
	}
	
	void Sound::Dispose() {
		
		// Release global source.
		s_GlobalSource.reset();
		
		// Release AL context.
	    alcMakeContextCurrent(nullptr);
		
		if (s_Context != nullptr) { alcDestroyContext(s_Context); s_Context = nullptr; }
		if (s_Device  != nullptr) { alcCloseDevice(s_Device);     s_Device  = nullptr; }
		
		if (s_SDL_Device > 0u) { SDL_CloseAudioDevice(s_SDL_Device); s_SDL_Device = 0u; }
	}
	
} // LouiEriksson