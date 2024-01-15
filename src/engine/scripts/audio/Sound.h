#ifndef FINALYEARPROJECT_SOUND_H
#define FINALYEARPROJECT_SOUND_H

#include <al.h>
#include <alc.h>

#include <memory>

namespace LouiEriksson::Audio {

	class AudioClip;
	class AudioSource;
	
	class Sound {
	
		/*
		 * Please refer to OpenAL-Soft spec:
		 * https://github.com/kcat/openal-soft/wiki/Programmer%27s-Guide
		 */
	
	private:
		
		/// <summary> Currently opened device as used by AL. </summary>
		inline static ALCdevice*  s_Device { nullptr };
		
		/// <summary> Current AL context. </summary>
		inline static ALCcontext* s_Context { nullptr };
		
		/// <summary>
		/// Currently opened audio device as used by the SDL fallback.
		/// <para><remarks>See also: s_Device.</para></remarks>
		/// </para>
		/// <summary>
		inline static unsigned int s_SDL_Device { 0u };
		
		inline static std::shared_ptr<AudioSource> s_GlobalSource;
		
	public:
	
		/// <summary> Initialise the audio subsystems. </summary>
		static void Init();
	
		/// <summary> Play the Clip without positional audio. </summary>
		static void PlayGlobal(const std::weak_ptr<AudioClip>& _clip);
		
		/// <summary> Set the distance model used by the audio engine. </summary>
		static void DistanceModel(const ALenum& _value);
		
		/// <summary> Get the distance model used by the audio engine. </summary>
		[[nodiscard]] ALenum DistanceModel() const;
		
		/// <summary>
		/// Set the doppler factor used by the audio engine.
		/// Must be greater than or equal to zero.
		/// </summary>
		static void DopplerFactor(const float& _value);
		
		/// <summary> Get the doppler factor used by the audio engine. </summary>
		[[nodiscard]] float DopplerFactor() const;
		
		/// <summary>
		/// Set the speed of sound used by the audio engine.
		/// This influences the doppler effect.
		/// Must be greater than or equal to zero.
		/// </summary>
		static void SpeedOfSound(const float& _value);
		
		/// <summary> Get the speed of sound used by the audio engine. </summary>
		[[nodiscard]] float SpeedOfSound() const;
		
		/// <summary> Finalise the audio subsystems. </summary>
		static void Dispose();
		
	};
	
} // LouiEriksson::Audio

#endif //FINALYEARPROJECT_SOUND_H