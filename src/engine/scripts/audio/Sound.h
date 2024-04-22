#ifndef FINALYEARPROJECT_SOUND_H
#define FINALYEARPROJECT_SOUND_H


#include "../core/Debug.h"

#include <al.h>
#include <alc.h>
#include <SDL.h>
#include <SDL_audio.h>

#include <algorithm>
#include <exception>
#include <stdexcept>

namespace LouiEriksson::Engine::Audio {

	class Sound final {
	
		friend class AudioSource;
		
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
		
	public:
		
		/** @brief Initialise the audio subsystems. */
		static void Init() {
			
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
					DopplerFactor(1.0);
					SpeedOfSound(343.3);
					
					Debug::Log("Done.", LogType::Info);
				}
				catch (const std::exception& e) {
					Debug::Log("Failed.", LogType::Error);
					
					throw e;
				}
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
		}
		
		/**
		 * @brief Set the distance model used by the audio engine.
		 *
		 * This function sets the distance model used by the audio engine. The distance
		 * model defines how the audio attenuates with distance.
		 *
		 * @param[in] _value The distance model to be set.
		 */
		inline static void DistanceModel(const ALenum& _value) {
			alDistanceModel(_value);
		}
		
		/**
		 * @fn ALenum Sound::DistanceModel()
		 * @brief Get the distance model used by the audio engine.
		 * @return The distance model used by the audio engine.
		 */
		[[nodiscard]] inline static ALenum DistanceModel() {
			return alGetInteger(AL_DISTANCE_MODEL);
		}
		
		/**
		 * @brief Set the doppler factor used by the audio engine.
		 * @param[in] _value The doppler factor value to be set. Must be greater than or equal to zero.
		 * @details The doppler factor determines the amount of pitch shift applied to audio sources based on their velocity relative to the listener's position.
		 * A value of 1.0 represents no shift, while a value greater than 1.0 increases the shift and a value less than 1.0 decreases the shift.
		 * @note The value must be greater than or equal to zero.
		 * @see SpeedOfSound()
		 * @warning This function modifies the doppler factor for all audio sources in the audio engine.
		 */
		inline static void DopplerFactor(const ALfloat& _value) {
			alDopplerFactor(std::max(_value, static_cast<ALfloat>(0.0)));
		}
		
		/**
		 * @brief Get the doppler factor used by the audio engine.
		 * @note This function returns the doppler factor value used by the audio engine.
		 * @return The doppler factor used by the audio engine.
		 */
		[[nodiscard]] inline static ALfloat DopplerFactor() {
			return alGetFloat(AL_DOPPLER_FACTOR);
		}
		
		/**
		 * @brief Set the speed of sound used by the audio engine.
		 *
		 * This function sets the speed of sound used by the audio engine. It influences the doppler effect.
		 *
		 * @param[in] _value The speed of sound value to be set.
		 * @note The value must be greater than or equal to zero.
		 * @see DopplerFactor()
		 */
		inline static void SpeedOfSound(const ALfloat& _value){
			alDopplerVelocity(std::max(_value, static_cast<ALfloat>(0.0)));
		}
		
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
		[[nodiscard]] inline static ALfloat SpeedOfSound() {
			return alGetFloat(AL_DOPPLER_VELOCITY);
		}
		
		/**
		 * @brief Finalise the audio subsystems.
		 *
		 * @note This function catches any exceptions thrown during the cleanup process and logs them as critical errors.
		 */
		inline static void Dispose() {
			
			try {
				
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
		
	};
	
} // LouiEriksson::Engine::Audio

#endif //FINALYEARPROJECT_SOUND_H