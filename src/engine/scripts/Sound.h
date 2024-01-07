#ifndef FINALYEARPROJECT_SOUND_H
#define FINALYEARPROJECT_SOUND_H

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
		inline static std::shared_ptr<AudioSource> s_GlobalSource = nullptr;
		
	public:
	
		/// <summary> Set the distance model used by the audio engine. </summary>
		static void DistanceModel(const ALenum& _value);
		
		/// <summary> Play the Clip without positional audio. </summary>
		static void PlayGlobal(const std::weak_ptr<AudioClip>& _clip);
		
		/// <summary> Initialise the audio subsystems. </summary>
		static void Init();
	
		/// <summary> Finalise the audio subsystems. </summary>
		static void Dispose();
		
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_SOUND_H