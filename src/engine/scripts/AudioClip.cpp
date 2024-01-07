#include "AudioClip.h"

namespace LouiEriksson {
	
	AudioClip::Format::Format(const SDL_AudioSpec& _audioSpec) {
		m_Specification = _audioSpec;
	}
	
	ALenum AudioClip::Format::OpenALFormat() const {
		
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
	
	AudioClip::AudioClip(const std::filesystem::path& _path) : m_Format({}) {
	
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
	
	AudioClip::~AudioClip() {
		Dispose();
	}
	
	void AudioClip::Dispose() {
		
		Free();
		
		// Delete the buffer.
		if (m_ALBuffer != AL_NONE) { alDeleteBuffers(1, &m_ALBuffer); m_ALBuffer = AL_NONE; }
	}
	
	void AudioClip::Free() {
		
		try {
			
			// Free data using SDL.
			if (m_Data != nullptr) { SDL_FreeWAV(m_Data); m_Data = nullptr; }
			
			m_Size = 0;
		}
		catch (const std::exception& e) {
			
			std::cout <<
				"Exception occurred when freeing allocated audio data. " <<
				"This is a potential memory leak!\n";
			
			std::cout << e.what() << '\n';
		}
	}
	
	
} // LouiEriksson