#ifndef FINALYEARPROJECT_SOUND_H
#define FINALYEARPROJECT_SOUND_H

#include "AudioSource.h"

#include <al.h>
#include <alc.h>

#include <memory>

namespace LouiEriksson::Engine::Audio {

	class AudioClip;
	class AudioSource;
	
	class Sound final {
	
		/*
		 * Please refer to OpenAL-Soft spec:
		 * https://github.com/kcat/openal-soft/wiki/Programmer%27s-Guide
		 */
	
	private:
		
		/** @brief Currently opened device as used by AL. */
		inline static ALCdevice*  s_Device { nullptr };
		
		/** @brief Current AL context. */
		inline static ALCcontext* s_Context { nullptr };
		
		/**
		 * @brief A static member representing the currently opened audio device ID when using the SDL fallback.
		 * @details This is used as a fallback option for audio output when the OpenAL Soft device (s_Device) can't be used.
		 * @see s_Device for the OpenAL Soft device equivalent.
		 */
		inline static unsigned int s_SDL_Device { 0u };
		
		/**
		 * @brief Global audio source used for non-positional audio playback.
		 * @note This is the audio source used for playback when AL is unavailable.
		 */
		inline static std::unique_ptr<AudioSource> s_GlobalSource;
		
	public:
		
		/** @brief Initialise the audio subsystems. */
		static void Init();
		
		/**
		 * @brief Play the Clip without positional audio.
		 *
		 * This function plays the given audio clip without positional audio.
		 *
		 * @param[in] _clip The audio clip to be played.
		 * @note This function throws a std::runtime_error if any error occurs during playback.
		 */
		static void PlayGlobal(const std::weak_ptr<AudioClip>& _clip);
		
		/**
		 * @brief Set the distance model used by the audio engine.
		 *
		 * This function sets the distance model used by the audio engine. The distance
		 * model defines how the audio attenuates with distance.
		 *
		 * @param[in] _value The distance model to be set.
		 */
		static void DistanceModel(const ALenum& _value);
		
		/**
		 * @fn ALenum Sound::DistanceModel()
		 * @brief Get the distance model used by the audio engine.
		 * @return The distance model used by the audio engine.
		 */
		[[nodiscard]] static ALenum DistanceModel() ;
		
		/**
		 * @brief Set the doppler factor used by the audio engine.
		 * @param[in] _value The doppler factor value to be set. Must be greater than or equal to zero.
		 * @details The doppler factor determines the amount of pitch shift applied to audio sources based on their velocity relative to the listener's position.
		 * A value of 1.0 represents no shift, while a value greater than 1.0 increases the shift and a value less than 1.0 decreases the shift.
		 * @note The value must be greater than or equal to zero.
		 * @see SpeedOfSound()
		 * @warning This function modifies the doppler factor for all audio sources in the audio engine.
		 */
		static void DopplerFactor(const ALfloat& _value);
		
		/**
		 * @brief Get the doppler factor used by the audio engine.
		 * @note This function returns the doppler factor value used by the audio engine.
		 * @return The doppler factor used by the audio engine.
		 */
		[[nodiscard]] static ALfloat DopplerFactor() ;
		
		/**
		 * @brief Set the speed of sound used by the audio engine.
		 *
		 * This function sets the speed of sound used by the audio engine. It influences the doppler effect.
		 *
		 * @param[in] _value The speed of sound value to be set.
		 * @note The value must be greater than or equal to zero.
		 * @see DopplerFactor()
		 */
		static void SpeedOfSound(const ALfloat& _value);
		
		/**
		 * @brief Get the speed of sound used by the audio engine.
		 *
		 * This function retrieves the speed of sound used by the audio engine.
		 * The speed of sound influences the doppler effect.
		 *
		 * @return The speed of sound used by the audio engine.
		 *
		 * @see Sound::DopplerFactor()
		 */
		[[nodiscard]] static ALfloat SpeedOfSound() ;
		
		/**
		 * @brief Finalise the audio subsystems.
		 *
		 * @note This function catches any exceptions thrown during the cleanup process and logs them as critical errors.
		 */
		static void Dispose();
		
	};
	
} // LouiEriksson::Engine::Audio

#endif //FINALYEARPROJECT_SOUND_H