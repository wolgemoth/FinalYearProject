#ifndef FINALYEARPROJECT_SOUND_H
#define FINALYEARPROJECT_SOUND_H

#include "stdafx.h"

#include "AudioSource.h"

namespace LouiEriksson {

	class Sound {
	
		/*
		 * Please refer to OpenAL-Soft spec:
		 * https://github.com/kcat/openal-soft/wiki/Programmer%27s-Guide
		 */
	
	private:
		
		/// <summary> Currently opened device as used by AL. </summary>
		inline static ALCdevice*  s_Device  = nullptr;
		
		/// <summary> Current AL context. </summary>
		inline static ALCcontext* s_Context = nullptr;
		
		/// <summary>
		/// Currently opened audio device as used by the SDL fallback.
		/// <para><remarks>See also: s_Device.</para></remarks>
		/// </para>
		/// <summary>
		inline static unsigned int s_SDL_Device = 0u;
		
		/// <summary> Global audio source. TODO: Implement this better. </summary>
		static std::shared_ptr<AudioSource> s_GlobalSource;
		
	public:
	
		struct Clip {
			
			/// <summary>
			/// Represents the format of an audio clip.
			/// Please note that positional audio does not work for sound files loaded in stereo.
			/// </summary>
			struct Format {
			
				/// <inheritdoc/>
				SDL_AudioSpec m_Specification;
				
				/// Get the OpenAL representation of the format.
				[[nodiscard]] ALenum OpenALFormat() const;
				
				explicit Format(const SDL_AudioSpec& _audioSpec);
			};
			
			/// <summary> The format of the audio clip. </summary>
			Format m_Format;
			
			/// <summary>
			/// Identifier of this clip's audio buffer as used by OpenAL.
			/// Please note that a value of 0u (or AL_NONE) indicates that
			/// the buffer has not yet initialised or initialised incorrectly.
			/// </summary>
			ALuint m_ALBuffer;
			
			/// <summary> Raw pointer to the underlying data of the sound file. </summary>
			Uint8* m_Data;
			
			/// <summary> Length of m_Data. </summary>
			Uint32 m_Size;
			
			explicit Clip(const std::filesystem::path& _path);
			
			~Clip();
			
			/// <summary>
			/// Releases the underlying data of the sound file.
			/// This happens automatically when the clip is disposed, and should
			/// not be called manually unless you know what you are doing.
			/// </summary>
			void Free();
			
			/// <summary> Dispose the clip. </summary>
			void Dispose();
		};
		
		/// <summary> Set the distance model used by the audio engine. </summary>
		static void DistanceModel(const ALenum& _value);
		
		/// <summary> Play the Clip without positional audio. </summary>
		static void PlayGlobal(const std::weak_ptr<Sound::Clip>& _clip);
		
		/// <summary> Initialise the audio subsystems. </summary>
		static void Init();
	
		/// <summary> Finalise the audio subsystems. </summary>
		static void Dispose();
		
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_SOUND_H