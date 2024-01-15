#ifndef FINALYEARPROJECT_AUDIOCLIP_H
#define FINALYEARPROJECT_AUDIOCLIP_H

#include <al.h>
#include <SDL_audio.h>
#include <SDL_stdinc.h>

#include <filesystem>

namespace LouiEriksson::Audio {
	
	class AudioClip {
	
		/*
		 * Please refer to OpenAL-Soft spec:
		 * https://github.com/kcat/openal-soft/wiki/Programmer%27s-Guide
		 */
	
	public:
		
		/// <summary>
		/// Represents the format of an audio clip.
		/// Please note that positional audio does not work for sound files loaded in stereo.
		/// </summary>
		struct Format {
		
			/// <inheritdoc/>
			SDL_AudioSpec m_Specification;
			
			/// Get the OpenAL representation of the format.
			[[nodiscard]] ALenum OpenALFormat() const;
			
			explicit Format(const SDL_AudioSpec& _audioSpec) noexcept;
			
		};
		
		/// <summary>
		/// Contains the samples of the audio clip.
		/// </summary>
		struct Samples {
			
			/// <summary> Raw pointer to the underlying data of the sound file. </summary>
			Uint8* m_Data;
			
			/// <summary> Length of m_Data. </summary>
			Uint32 m_Length;
			
			/// <summary>
			/// Releases the underlying samples of the sound file.
			/// This happens automatically when the clip is disposed, and should
			/// not be called manually unless you know what you are doing.
			/// </summary>
			void Free();
		};
		
		/// <summary> The format of the audio clip. </summary>
		Format m_Format;
		
		/// <summary> The samples of the audio clip. </summary>
		Samples m_Samples;
		
		/// <summary>
		/// Identifier of this clip's audio buffer as used by OpenAL.
		/// Please note that a value of 0u (or AL_NONE) indicates that
		/// the buffer has not yet initialised or initialised incorrectly.
		/// </summary>
		ALuint m_ALBuffer;
		
		/// <summary> Duration of the clip in seconds. </summary>
		float m_Duration;
		
		explicit AudioClip(const std::filesystem::path& _path);
		~AudioClip();
		
		/// <summary> Dispose the clip. </summary>
		void Dispose();
		
	};
	
} // LouiEriksson::Audio

#endif //FINALYEARPROJECT_AUDIOCLIP_H