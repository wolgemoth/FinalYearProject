#ifndef FINALYEARPROJECT_AUDIOCLIP_H
#define FINALYEARPROJECT_AUDIOCLIP_H

#include <al.h>
#include <SDL_audio.h>
#include <SDL_stdinc.h>

#include <filesystem>

namespace LouiEriksson::Engine::Audio {

/**
 * @class AudioClip
 * @brief This class is responsible for loading and management of audio clips. Currently only supports loading of WAV files.
 *
 * @note Positional audio will not work for sound files loaded in stereo.
 *
 * @see Please kindly refer to the <a href="https://github.com/kcat/openal-soft/wiki/Programmer%27s-Guide">OpenAL-Soft spec</a> for more information on OpenAL.
 */
	class AudioClip {
	
	public:
		
		/**
		 * @class Format
		 * @brief Represents the format of an audio clip.
		 * @note Positional audio will not work for sound files loaded in stereo.
		 * @see SDL_AudioSpec
		 */
		struct Format final {
		
			/** @inheritdoc */
			SDL_AudioSpec m_Specification;
			
			/**
			 * @brief Get the OpenAL format of the audio clip format.
			 *
			 * This function returns the corresponding OpenAL format for the audio clip format.
			 *
			 * @return The OpenAL format as an ALenum value.
			 * @note This function will log an error if the audio clip format is unsupported.
			 * @see ALenum
			 */
			[[nodiscard]] ALenum OpenALFormat() const;
			
			explicit Format(const SDL_AudioSpec& _audioSpec) noexcept;
			
		};
		
		/**
		 * @brief Contains the samples of the audio clip.
		 */
		struct Samples {
			
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
			void Free();
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
		explicit AudioClip(const std::filesystem::path& _path);
		~AudioClip();
		
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
		void Dispose();
	};
	
} // LouiEriksson::Engine::Audio

#endif //FINALYEARPROJECT_AUDIOCLIP_H