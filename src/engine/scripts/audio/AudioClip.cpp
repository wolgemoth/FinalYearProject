#include "AudioClip.h"

#include <AL/al.h>
#include <SDL_audio.h>

#include <exception>
#include <filesystem>
#include <iostream>

namespace LouiEriksson::Audio {
	
	AudioClip::Format::Format(const SDL_AudioSpec& _audioSpec) noexcept :
			m_Specification(_audioSpec) {}
	
	ALenum AudioClip::Format::OpenALFormat() const {
		
		auto result = AL_NONE;
		
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
	
	void AudioClip::Samples::Free() {
		
		try {
			
			// Free data using SDL.
			if (m_Data != nullptr) { SDL_FreeWAV(m_Data); m_Data = nullptr; }
			
			m_Length = 0;
		}
		catch (const std::exception& e) {
			
			std::cout <<
				"Exception occurred when freeing allocated audio data. " <<
				"This is a potential memory leak!\n";
			
			std::cout << e.what() << '\n';
		}
	}
	
	AudioClip::AudioClip(const std::filesystem::path& _path) :
			m_Format({}),
			m_Samples(),
			m_ALBuffer(AL_NONE)
	{
		// Generate audio buffer.
		alGenBuffers(1, &m_ALBuffer);
	
		{
			SDL_AudioSpec spec;
			
			// Load the audio data into a c-style byte array using SDL.
			SDL_LoadWAV(_path.c_str(), &spec, &m_Samples.m_Data, &m_Samples.m_Length);
			
			m_Format = Format(spec);

			// Compute the duration of the audio file by performing the following operation:
			m_Duration = static_cast<float>(m_Samples.m_Length) /                // Sample Count
					     static_cast<float>(spec.freq)          /                // Sample Rate
					     static_cast<float>(spec.channels)      /                // Channel Count
						 static_cast<float>(SDL_AUDIO_BITSIZE(spec.format) / 8); // Stride
		}
		
		// Buffer the audio data and free the loaded data.
		if (m_ALBuffer != AL_NONE) {
	
			alBufferData(
				m_ALBuffer,
				m_Format.OpenALFormat(),
				m_Samples.m_Data,
				static_cast<ALsizei>(m_Samples.m_Length),
				m_Format.m_Specification.freq
			);
		}
	}
	
	AudioClip::~AudioClip() {
		Dispose();
	}
	
	void AudioClip::Dispose() {
		
		m_Samples.Free();
		
		// Delete the buffer.
		if (m_ALBuffer != AL_NONE) { alDeleteBuffers(1, &m_ALBuffer); m_ALBuffer = AL_NONE; }
	}
	
} // LouiEriksson::Audio