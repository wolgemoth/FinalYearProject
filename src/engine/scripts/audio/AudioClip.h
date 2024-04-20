#ifndef FINALYEARPROJECT_AUDIOCLIP_H
#define FINALYEARPROJECT_AUDIOCLIP_H

#include "../core/Debug.h"

#include <al.h>
#include <SDL_audio.h>
#include <SDL_stdinc.h>

#include <filesystem>
#include <string>

namespace LouiEriksson::Engine::Audio {

/**
 * @class AudioClip
 * @brief This class is responsible for loading and management of audio clips. Currently only supports loading of WAV files.
 *
 * @note Positional audio will not work for sound files loaded in stereo.
 *
 * @see Please kindly refer to the <a href="https://github.com/kcat/openal-soft/wiki/Programmer%27s-Guide">OpenAL-Soft spec</a> for more information on OpenAL.
 */
	class AudioClip final {
	
	public:
		
		/**
		 * @class Format
		 * @brief Represents the format of an audio clip.
		 * @note Positional audio will not work for sound files loaded in stereo.
		 * @see SDL_AudioSpec
		 */
		struct Format final {
		
			friend AudioClip;
			
		private:
		
			explicit constexpr Format(const SDL_AudioSpec& _audioSpec) noexcept :
				m_Specification(_audioSpec) {}
				
			/** @inheritdoc */
			SDL_AudioSpec m_Specification;
			
		public:
			
			Format(const Format& _other) = delete;
			
			Format& operator = (const Format&  _other) = delete;
			Format& operator =       (Format&& _other) = delete;
			
			constexpr const SDL_AudioSpec& Specification() const {
				return m_Specification;
			}

			/**
			 * @brief Get the OpenAL format of the audio clip format.
			 *
			 * This function returns the corresponding OpenAL format for the audio clip format.
			 *
			 * @return The OpenAL format as an ALenum value.
			 * @note This function will log an error if the audio clip format is unsupported.
			 * @see ALenum
			 */
			[[nodiscard]] constexpr ALenum OpenALFormat() const noexcept {
				
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
							Debug::Log("Unimplemented format: " + std::to_string(m_Specification.format), LogType::Error);
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
							Debug::Log("Unimplemented format:" + std::to_string(m_Specification.format), LogType::Error);
						}
					}
				}
				
				return result;
			}
			
		};
		
		/**
		 * @brief Contains the samples of the audio clip.
		 */
		struct Samples final {
		
			friend AudioClip;
			
		private:
			
			Samples() :
				m_Data(nullptr),
				m_Length(0) {}
			
		public:
			
			Samples(const Samples& _other) = delete;
			
			Samples& operator = (const Samples&  _other) = delete;
			Samples& operator =       (Samples&& _other) = delete;
			
			/** @brief Raw pointer to the underlying data of the sound file. */
			Uint8* m_Data;
			
			/** @brief Length of m_Data. */
			Uint32 m_Length;
			
			/**
			 * @brief Releases the underlying samples of the sound file.
			 *
			 * This function frees the memory allocated for the sound file samples.
			 * It is called automatically when the AudioClip object is disposed,
			 * and should not need to be called manually.
			 *
			 * @see AudioClip::Dispose()
			 */
			void Free() {
		
				try {
					
					// Free data using SDL.
					if (m_Data != nullptr) { SDL_FreeWAV(m_Data); m_Data = nullptr; }
					
					m_Length = 0;
				}
				catch (const std::exception& e) {
					
					Debug::Log(
						std::string("Exception occurred when freeing allocated audio data. This is a potential memory leak! ") + e.what(),
						LogType::Critical
					);
				}
			}
		};
		
		/** @brief The format of the audio clip. */
		Format m_Format;
		
		/** @brief The samples of the audio clip. */
		Samples m_Samples;
		
		/**
		 * @brief Identifier of this clip's audio buffer as used by OpenAL.
		 *
		 * @note A value of 0u (or AL_NONE) indicates that
		 * the buffer has not yet initialised or initialised incorrectly.
		 */
		ALuint m_ALBuffer;
		
		/** @brief Duration of the clip in seconds. */
		float m_Duration;
		
		/**
		 * @class AudioClip
		 * @brief Represents an audio clip.
		 *
		 * The AudioClip class is responsible for loading and managing audio clips. It uses SDL to load the audio data and OpenAL for audio playback.
		 *
		 * @note The AudioClip class assumes that the audio file is in a supported format. Positional audio will not work for sound files loaded in stereo.
		 */
		explicit AudioClip(const std::filesystem::path& _path) :
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
				
				m_Format.m_Specification = spec;
	
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
		
		~AudioClip() {
			Dispose();
		}
		
		/**
		 * @brief Releases the resources used by the audio clip.
		 *
		 * This function frees the memory allocated for the sound file samples and
		 * deletes the audio buffer used by OpenAL.
		 *
		 * @note After calling this function, the AudioClip object should not be used
		 * anymore, as it will be in an invalid state.
		 * @see AudioClip::Samples::Free(), alDeleteBuffers()
		 */
		void Dispose() {
			
			try {
				m_Samples.Free();
				
				// Delete the buffer.
				if (m_ALBuffer != AL_NONE) { alDeleteBuffers(1, &m_ALBuffer); m_ALBuffer = AL_NONE; }
			}
			catch (const std::exception& e) {
				Debug::Log(e, LogType::Critical);
			}
		}
	};
	
} // LouiEriksson::Engine::Audio

#endif //FINALYEARPROJECT_AUDIOCLIP_H