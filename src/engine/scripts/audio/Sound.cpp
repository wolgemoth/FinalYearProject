#include "Sound.h"

#include "AudioClip.h"
#include "AudioSource.h"
#include "../core/Debug.h"

#include <al.h>
#include <alc.h>
#include <glm/common.hpp>
#include <SDL.h>
#include <SDL_audio.h>

#include <exception>
#include <memory>
#include <stdexcept>

namespace LouiEriksson::Engine::Audio {
	
	void Sound::Init() {
		
		Debug::Log("Initialising audio subsystems...", LogType::Info);
		
		try {
			
			// Init SDL audio subsystem (as fallback).
			try {
				Debug::Log("\tSDL...", LogType::Info, true);
				SDL_InitSubSystem(SDL_INIT_AUDIO);
				Debug::Log("Done.", LogType::Info);
			}
			catch (const std::exception& e) {
				Debug::Log("Failed.", LogType::Error);
				
				throw e;
			}
			
			// Init OpenAL audio subsystem.
			try {
				
				Debug::Log("\tOpenAL...", LogType::Info, true);
			
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
				
				// Set the default distance model for the audio context.
				// See: https://github.com/kcat/openal-soft/wiki/Programmer%27s-Guide#aldistancemodel
				DistanceModel(AL_INVERSE_DISTANCE);
				
				// Set default values for doppler shift.
				// See: https://github.com/kcat/openal-soft/wiki/Programmer%27s-Guide#doppler-shift
				DopplerFactor(1.0f);
				SpeedOfSound(343.3f);
				
				Debug::Log("Done.", LogType::Info);
			}
			catch (const std::exception& e) {
				Debug::Log("Failed.", LogType::Error);
				
				throw e;
			}
			
			/*
			 * Generate the global audio source.
			 * This will be responsible for on-demand playback of non-positional sound.
			 */
			s_GlobalSource.reset(new AudioSource({}));
			s_GlobalSource->Global(true);
			
		}
		catch (const std::exception& e) {
			Debug::Log(e);
		}
	}
	
	void Sound::PlayGlobal(const std::weak_ptr<AudioClip>& _clip) {
	
		try {
			
			if (s_GlobalSource != nullptr) {
			
				if (const auto c = _clip.lock()) {
				
					// Only play if the clip actually contains data.
					if (c->m_Samples.m_Length > 0) {
					
						if (c->m_ALBuffer == AL_NONE) {
							
							// Use SDL2 as fallback if AL buffer is uninitialised.
							if (s_SDL_Device > 0u) { SDL_CloseAudioDevice(s_SDL_Device); s_SDL_Device = 0u; }
							
							s_SDL_Device = SDL_OpenAudioDevice(nullptr, 0, &c->m_Format.m_Specification, nullptr, 0);
							
							// Play using SDL!
							SDL_QueueAudio(s_SDL_Device, c->m_Samples.m_Data, c->m_Samples.m_Length);
							SDL_PauseAudioDevice(s_SDL_Device, 0);
						}
						else {
							
							// Stop the source if it is already playing.
							if (s_GlobalSource->State() == AL_PLAYING) {
								s_GlobalSource->Stop();
							}
							
							// Assign the new clip.
							s_GlobalSource->Clip(c);
							
							// Play! (Set fallback to false as we have already established it is not necessary.)
							s_GlobalSource->Play(false);
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
			else {
				throw std::runtime_error("Cannot play since s_Global_Source is nullptr!");
			}
		}
		catch (const std::exception& e) {
			Debug::Log(e);
		}
	}
	
	void Sound::DistanceModel(const ALenum& _value) {
		alDistanceModel(_value);
	}
	ALenum Sound::DistanceModel() {
		return alGetInteger(AL_DISTANCE_MODEL);
	}
	
	void Sound::DopplerFactor(const float& _value) {
		alDopplerFactor(glm::max(_value, 0.0f));
	}
	float Sound::DopplerFactor() {
		return alGetFloat(AL_DOPPLER_FACTOR);
	}
	
	void Sound::SpeedOfSound(const float& _value) {
		alDopplerVelocity(glm::max(_value, 0.0f));
	}
	float Sound::SpeedOfSound() {
		return alGetFloat(AL_DOPPLER_VELOCITY);
	}
	
	void Sound::Dispose() {
		
		try {
			
			// Release global source.
			s_GlobalSource.reset();
			
			// Release AL context.
		    alcMakeContextCurrent(nullptr);
			
			if (s_Context != nullptr) { alcDestroyContext(s_Context); s_Context = nullptr; }
			if (s_Device  != nullptr) { alcCloseDevice(s_Device);     s_Device  = nullptr; }
			
			if (s_SDL_Device > 0u) { SDL_CloseAudioDevice(s_SDL_Device); s_SDL_Device = 0u; }
		}
		catch (const std::exception& e) {
			Debug::Log(e, LogType::Critical);
		}
	}
	
} // LouiEriksson::Engine::Audio