#ifndef FINALYEARPROJECT_AUDIOSOURCE_H
#define FINALYEARPROJECT_AUDIOSOURCE_H

#include "../audio/Sound.h"

#include "../core/Debug.h"
#include "../core/Script.h"
#include "../core/Time.h"
#include "../core/Transform.h"
#include "../ecs/GameObject.h"
#include "../physics/Rigidbody.h"

#include <al.h>
#include <algorithm>
#include <glm/common.hpp>

#include <exception>
#include <limits>
#include <memory>
#include <stdexcept>

namespace LouiEriksson::Engine::Audio {
	
	class Sound;
	
	/**
	 * @class AudioSource
	 * @brief Represents an audio source in the scene.
	 *
	 * This class is responsible for managing the state and properties of an audio source.
	 *
	 * @see Please kindly refer to the <a href="https://github.com/kcat/openal-soft/wiki/Programmer%27s-Guide">OpenAL-Soft spec</a> for more information on OpenAL.
	 */
	class AudioSource final : public Script {
	
	private:
		
		/**
		 * @brief Global audio source used for non-positional audio playback.
		 * @note This is the audio source used for playback when AL is unavailable.
		 */
		inline static std::unique_ptr<AudioSource> s_GlobalSource;
		
		/**
		 * @struct Parameters
		 * @brief Represents the parameters for an AudioSource.
		 *
		 * This struct stores various properties related to audio playback.
		 */
		struct Parameters final {
		
			/**
			 * @brief Determines whether the AudioSource is global or not.
			 * @note Global AudioSources do not play positional audio.
			 */
			bool m_IsGlobal;
			
			/**
			 * @brief Determines whether or not the AudioSource should loop the current clip.
			 */
			bool m_Loop;
			
			/**
			 * @brief Panning of audio emitted from the AudioSource.
			 * @note This only has an effect on global sources.
			 */
			glm::vec3 m_Panning;
			
			/**
			 * @brief Minimum distance for this AudioSource.
			 *
			 * If the distance between the listener and the AudioSource is below this level,
			 * audio will be emitted from this AudioSource at the maximum gain value.
			 *
			 * @note Must be a value greater than zero.
			 */
			ALfloat m_MinDistance;
			
			/**
			 * @brief Maximum distance for this AudioSource.
			 *
			 * If the distance between the listener and the AudioSource is above this level,
			 * audio will be emitted from this AudioSource at the minimum gain value.
			 *
			 * @note Must be a value greater than or equal to the minimum distance.
			 */
			ALfloat m_MaxDistance;
			
			/**
			 * @brief Pitch multiplier of this AudioSource.
			 * @note Must be a value greater than zero.
			 */
			ALfloat m_Pitch;
			
			/**
			 * @brief Gain modifier for this AudioSource.
			 * @note Must be a value greater than or equal to zero.
			 */
			ALfloat m_GainModifier;
			
			/**
			 * @brief Minimum gain for this AudioSource.
			 * @note Must be a value greater than or equal to zero.
			 */
			ALfloat m_MinGain;
			
			/**
			 * @brief Maximum gain for this AudioSource.
			 * @note Must be a value greater than or equal to the minimum gain.
			 */
			ALfloat m_MaxGain;
			
			/**
			 * @brief Rolloff factor of this AudioSource.
			 */
			ALfloat m_Rolloff;
			
			/**
			 * @brief Minimum angle of the AudioSource.
			 *
			 * By default this value is 360 degrees, for omnidirectional playback.
			 * Values less than 360 degrees cause the AudioSource to play directionally.
			 *
			 * @note Must be a value greater than or equal to zero.
			 */
			ALfloat m_MinAngle;
			
			/**
			 * @brief Maximum angle of the AudioSource.
			 * By default this value is 360 degrees.
			 *
			 * @note Must be a value greater than or equal to the minimum angle.
			 */
			ALfloat m_MaxAngle;
			
			Parameters() noexcept :
				m_IsGlobal(false), // Do not start as global.
				m_Loop    (false), // Do not loop.
				
				// Default panning value. (Will only apply if if the audio source is global.)
				m_Panning( 0.0),
				
				// Set default minimum and maximum distances.
				m_MinDistance(  1.0),
				m_MaxDistance(100.0),
				
				// Set other defaults:
				m_Pitch       (  1.0),
				m_GainModifier(  1.0),
				m_MinGain     (  0.0),
				m_MaxGain     (  1.0),
				m_Rolloff     (  1.0),
				m_MinAngle    (360.0),
				m_MaxAngle    (360.0) {}
		};
		
		/**
		 * @brief Identifier of this source as used by OpenAL.
		 * @note A value of 0u (or AL_NONE) indicates that the source has not yet initialised or initialised incorrectly.
		 */
		ALuint m_Source;
		
		/** @brief Parameters of the source. */
		Parameters m_Parameters;
		
		/** @brief Most recently played AudioClip. */
		std::weak_ptr<AudioClip> m_Clip;
		
		/** @brief Last position of the AudioSource (for computing transform-based velocity). */
		glm::vec3 m_LastPosition;
		
		/** @brief Synchronise the AudioSource with the internal audio engine. */
		void Sync() {
			
			// TODO: Some sort of caching so these values aren't changed unnecessarily.
			
			// Set values that do not change depending on if the source is global or not:
			alSourcef(m_Source, AL_PITCH,          m_Parameters.m_Pitch);
			alSourcef(m_Source, AL_GAIN,           m_Parameters.m_GainModifier);
			alSourcef(m_Source, AL_MIN_GAIN,       m_Parameters.m_MinGain);
			alSourcef(m_Source, AL_MAX_GAIN,       m_Parameters.m_MaxGain);
			alSourcef(m_Source, AL_ROLLOFF_FACTOR, m_Parameters.m_Rolloff);
			
			// If the audio source is global...
			if (m_Parameters.m_IsGlobal) {
				
				/* NULLIFY EFFECT OF ORIENTATION */
		
				/*
				 * If AL_SOURCE_RELATIVE is true, the audio is played relative to the listener.
				 *
				 * In this case, the position of the source is (0.0, 0.0, 0.0) units
				 * from the listener's position (or directly on top of the listener).
				 *
				 * This will cause the panning of the audio to always be centered, regardless of
				 * the position or orientation of the audio listener.
				 */
				alSourcei(m_Source, AL_SOURCE_RELATIVE, AL_TRUE);
				
				/* SET PANNING */
				
				// Global AudioSources have relative position to the listener, so update local position only.
				alSourcefv(m_Source, AL_POSITION, static_cast<ALfloat*>(&m_Parameters.m_Panning[0]));
				
				/* DISABLE ATTENUATION */
				
				// Setting reference distance to 0.0 disables attenuation.
				alSourcef(m_Source, AL_REFERENCE_DISTANCE, 0.0);
				
				// Set max distance to infinity (not strictly-necessary).
				alSourcef(m_Source, AL_MAX_DISTANCE, std::numeric_limits<ALfloat>::max());
				
				// Nullify directionality of AudioSource
				alSourcef(m_Source, AL_CONE_OUTER_ANGLE, 360.0);
				alSourcef(m_Source, AL_CONE_OUTER_ANGLE, 360.0);
				
				/* RESET VELOCITY */
				alListener3f(AL_VELOCITY, 0.0, 0.0, 0.0);
			}
			else {
			
				// Set all defined parameters in a positional context:
				alSourcef(m_Source, AL_REFERENCE_DISTANCE, m_Parameters.m_MinDistance);
				alSourcef(m_Source, AL_MAX_DISTANCE,       m_Parameters.m_MaxDistance);
				
				// Set min and max cone angles of the source.
				alSourcef(m_Source, AL_CONE_OUTER_ANGLE, m_Parameters.m_MinAngle);
				alSourcef(m_Source, AL_CONE_OUTER_ANGLE, m_Parameters.m_MaxAngle);
				
				if (const auto p = Parent().lock()) {
				if (const auto t = p->GetComponent<Transform>().lock()) {
					
					// Set world position to that of the current transform.
					alSourcefv(m_Source, AL_POSITION, static_cast<const ALfloat*>(&(t->Position()[0])));
					
					// Set the direction of the audio source using the current transform.
					{
						const auto dir = t->FORWARD;
						
						alSource3f(m_Source, AL_DIRECTION, dir.x, dir.y, dir.z);
					}
				
					// Set velocity (using rigidbody, if available):
					{
						glm::vec3 velocity;
						
						if (const auto r = p->GetComponent<Physics::Rigidbody>().lock()) {
							velocity = r->Velocity();
						}
						else {
							velocity = (t->Position() - m_LastPosition) * Time::DeltaTime<scalar_t >();
						}
						
						alListenerfv(AL_VELOCITY, static_cast<ALfloat*>(&velocity[0]));
					}
					
					m_LastPosition = t->Position();
				}}
			}
		}
		
		/**
		 * @brief Play the Clip without positional audio.
		 *
		 * This function plays the given audio clip without positional audio.
		 *
		 * @param[in] _clip The audio clip to be played.
		 * @note This function throws a std::runtime_error if any error occurs during playback.
		 */
		inline static void PlayGlobal(const std::weak_ptr<AudioClip>& _clip){
			
			try {
				
				/*
				 * Generate the global audio source.
				 * This will be responsible for on-demand playback of non-positional sound.
				 */
				if (s_GlobalSource == nullptr) {
					s_GlobalSource.reset(new AudioSource({}));
					s_GlobalSource->Global(true);
				}
				
				if (s_GlobalSource != nullptr) {
				
					if (const auto c = _clip.lock()) {
					
						// Only play if the clip actually contains data.
						if (c->m_Samples.m_Length > 0) {
						
							if (c->m_ALBuffer == AL_NONE) {
								
								/* Use SDL2 as fallback if AL buffer is uninitialised. */
								
								if (Sound::s_SDL_Device > 0u) {
									SDL_CloseAudioDevice(Sound::s_SDL_Device);
									Sound::s_SDL_Device = 0u;
								}
								
								Sound::s_SDL_Device = SDL_OpenAudioDevice(nullptr, 0, &c->m_Format.Specification(), nullptr, 0);
								
								// Play using SDL!
								SDL_QueueAudio(Sound::s_SDL_Device, c->m_Samples.m_Data, c->m_Samples.m_Length);
								SDL_PauseAudioDevice(Sound::s_SDL_Device, 0);
							}
							else {
								
								// Stop the source if it is already playing.
								if (s_GlobalSource->State() == AL_PLAYING) {
									s_GlobalSource->Stop();
								}
								
								// Assign the new clip.
								s_GlobalSource->Clip(c);
								
								// Play! (Set fallback to false as we have already established it is not necessary.)
								s_GlobalSource->Play(false);
							}
						}
						else {
							throw std::runtime_error("Cannot play AudioClip since its size is zero!");
						}
					}
					else {
						throw std::runtime_error("Cannot play AudioSource since the current clip is nullptr!");
					}
				}
				else {
					throw std::runtime_error("Cannot play since s_Global_Source is nullptr!");
				}
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
		}
		
	public:
		
		explicit AudioSource(const std::weak_ptr<ECS::GameObject>& _parent) : Script(_parent),
			m_Source      (AL_NONE),
			m_Parameters  (),
			m_LastPosition(0.0)
		{
			try {
				
				if (m_Source == AL_NONE) {
					
					alGenSources(1, &m_Source);
					
					if (m_Source == AL_NONE) {
						throw std::runtime_error("Failed creating audio source!");
					}
					
					Sync();
				}
				else {
					throw std::runtime_error("Attempted to initialise an AudioSource which is already initialised!");
				}
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
		}
		 
		~AudioSource() override {
			
			if (m_Source != AL_NONE) {
				alDeleteSources(1, &m_Source);
			}
		}
		
		/** @inheritdoc */
		[[nodiscard]] inline std::type_index TypeID() const noexcept override { return typeid(AudioSource); };
		
		/** @brief Initialise the AudioSource. */
		inline void Begin() override {
			Sync();
		}
		
		/** @brief Updates the AudioSource every frame. */
		inline void Tick() override {
			Sync();
		}
		
		/**
		 * @brief Plays the audio clip associated with the AudioSource.
		 *
		 * @param[in] _allowFallback Determines whether to allow fallback playback if the audio clip's buffer is not available.
		 * If set to true and the buffer is not available, the clip will be played globally using a fallback method.
		 * If set to false and the buffer is not available, no playback will occur.
		 */
		void Play(const bool& _allowFallback = true) {
		
			try {
				
				// Try and get current clip.
				if (const auto c = m_Clip.lock()) {
					
					// Only play if the clip actually contains data.
					if (c->m_Samples.m_Length <= 0) {
						throw std::runtime_error("Cannot play AudioClip since its size is zero!");
					}
					else {
						
						if (c->m_ALBuffer == AL_NONE) {
							
							/*
							 * AL buffer non-existent. Implies AL failed to initialise correctly.
							 * Attempt to play the clip globally instead. This will use whatever fallback is available.
							 */
							
							if (_allowFallback) {
								PlayGlobal(c);
							}
							else {
								throw std::runtime_error(
										"Cannot play audio clip since m_ALBuffer == AL_NONE, "
										"and permission to use a fallback is denied!\n"
								);
							}
						}
						else {
							
							// If already playing, nothing needs to be done.
							if (State() == AL_PLAYING) {
								throw std::runtime_error("Attempted to call Play() on an AudioSource that is already playing.");
							}
							else {
								
								// Play!
								alSourcei(m_Source, AL_BUFFER, static_cast<ALint>(c->m_ALBuffer));
								alSourcePlay(m_Source);
							}
						}
					}
				}
				else {
					throw std::runtime_error("Cannot play AudioSource since the current clip is nullptr!");
				}
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
		}
		
		/** @brief Pause this AudioSource. */
		inline void Pause() const { alSourcePause(m_Source); }
		
		/** @brief Stop this AudioSource. */
		inline void Stop() const { alSourceStop(m_Source); }
		
		/**
		 * @brief Sets the AudioClip for the AudioSource.
		 *
		 * This function sets the AudioClip for the AudioSource. The AudioClip is stored as a weak pointer, which allows
		 * the AudioSource to access the AudioClip without taking ownership of it.
		 *
		 * @param[in] _value The AudioClip to set for the AudioSource.
		 */
		inline void Clip(const std::weak_ptr<AudioClip>& _value) noexcept{
			m_Clip = _value;
		}
		
		/**
		 * @brief Returns the AudioClip associated with the AudioSource.
		 *
		 * This function returns a weak pointer to the AudioClip associated with the AudioSource.
		 *
		 * @return A weak pointer to the AudioClip.
		 */
		[[nodiscard]] constexpr const std::weak_ptr<AudioClip>& Clip() const noexcept {
			return m_Clip;
		}
		
		/**
		 * @brief Returns the state of the AudioSource.
		 *
		 * This function retrieves the state of the AudioSource, which indicates whether it is playing, paused, or stopped.
		 * The state is returned as an ALenum value.
		 *
		 * @return The state of the AudioSource.
		 */
		[[nodiscard]] inline ALenum State() const {
			
			ALint state;
			
		    alGetSourcei(m_Source, AL_SOURCE_STATE, &state);
			
		    return static_cast<ALenum>(state);
		}
		
		/**
		 * @brief Sets the global flag of the AudioSource.
		 *
		 * This function sets the global flag of the AudioSource. A global AudioSource does not play positional audio.
		 *
		 * @param[in] _value The value to set the global flag to.
		 */
		inline void Global(const bool& _value) {
			m_Parameters.m_IsGlobal = _value;
			
			Sync();
		}
		
		/**
		 * @brief Returns the global flag of the AudioSource.
		 *
		 * This function returns a const reference to the global flag of the AudioSource.
		 * A global AudioSource does not play positional audio.
		 *
		 * @return A const reference to the global flag of the AudioSource.
		 */
		[[nodiscard]] constexpr const bool& Global() const noexcept  {
		return m_Parameters.m_IsGlobal;
	}
		
		/**
		 * @brief Sets whether the audio source should loop or not.
		 *
		 * This function sets whether the audio source should loop or not based on the provided value.
		 * When the audio source is set to loop, it will continuously play the audio until explicitly stopped.
		 *
		 * @param[in] _value The value indicating whether the audio source should loop or not.
		 *        - true to enable looping.
		 *        - false to disable looping.
		 *
		 * @see AudioSource::Sync()
		 */
		inline void Loop(const bool& _value){
			m_Parameters.m_Loop = _value;
			
			Sync();
		}
		
		/**
		 * @brief Get the loop flag of the audio source.
		 *
		 * This function returns a const reference to the loop flag of the audio source.
		 * The loop flag determines whether the audio source should loop playback or not.
		 *
		 * @return A const reference to the loop flag. True if loop is enabled, false otherwise.
		 */
		[[nodiscard]] constexpr const bool& Loop() const noexcept {
			return m_Parameters.m_Loop;
		}
		
		/**
		 * @brief Sets the minimum distance of the audio source.
		 *
		 * The minimum distance determines the distance at which the audio starts to attenuate.
		 * If the listener is closer to the audio source than the minimum distance, the volume of the audio will be at its maximum.
		 * When the listener moves farther away, the volume will start to attenuate.
		 *
		 * @param[in] _value The new minimum distance value to set.
		 * @note The value should be within the range of FLT_EPSILON to the maximum distance.
		 */
		inline void MinDistance(const ALfloat& _value){
			m_Parameters.m_MinDistance = std::max(_value, std::numeric_limits<ALfloat>::min());
			m_Parameters.m_MaxDistance = std::max(_value, m_Parameters.m_MaxDistance);
			
			Sync();
		}
		
		/**
		 * @brief Gets the minimum distance for the audio source.
		 *
		 * The minimum distance determines the distance at which the audio starts to attenuate.
		 * If the listener is closer to the audio source than the minimum distance, the volume of the audio will be at its maximum.
		 * When the listener moves farther away, the volume will start to attenuate.
		 *
		 * @note This function is noexcept and will not throw any exceptions.
		 *
		 * @return A const reference to the minimum distance.
		 */
		[[nodiscard]] constexpr const ALfloat& MinDistance() const noexcept {
			return m_Parameters.m_MinDistance;
		}
		
		/**
		 * @brief Sets the maximum distance for audio source.
		 *
		 * This function sets the maximum distance for the audio source. The maximum distance is the point at which the sound starts to attenuate and become quieter.
		 *
		 * @param[in] _value The new maximum distance.
		 *
		 * @note The _value is clamped to be greater than or equal to the minimum distance. If _value is smaller than the minimum distance, the maximum distance is set to be the same as the minimum distance.
		 */
		inline void MaxDistance(const ALfloat& _value) {
			m_Parameters.m_MaxDistance = std::max(_value, m_Parameters.m_MinDistance);
			
			Sync();
		}
		
		/**
		 * @brief Get the maximum distance of the audio source.
		 *
		 * This function returns the maximum distance at which the audio source can be heard.
		 * The maximum distance is a constant value and cannot be modified.
		 *
		 * @return const ALfloat& The maximum distance of the audio source.
		 */
		[[nodiscard]] constexpr const ALfloat& MaxDistance() const noexcept {
			return m_Parameters.m_MaxDistance;
		}
		
		/**
		 * @brief Sets the pitch of the audio source.
		 *
		 * The pitch determines the speed at which the audio is played.
		 *
		 * @param[in] _value The pitch value to be set. Must be a positive ALfloat.
		 */
		inline void Pitch(const ALfloat& _value) {
			m_Parameters.m_Pitch = std::max(_value, static_cast<ALfloat>(0.0));
			
			Sync();
		}
		
		/**
		 * @brief Get the pitch of the audio source.
		 *
		 * This function returns a constant reference to the pitch of the audio source.
		 *
		 * @note The pitch determines the playback speed of the audio source. A pitch of 1.0
		 *       represents normal speed, while a pitch greater than 1.0 increases the speed,
		 *       and a pitch less than 1.0 decreases the speed.
		 *
		 * @return A constant reference to the pitch of the audio source.
		 */
		[[nodiscard]] constexpr const ALfloat& Pitch() const noexcept {
			return m_Parameters.m_Pitch;
		}
		
		/**
		 * @brief Sets the gain modifier of the audio source.
		 *
		 * This function allows you to adjust the volume of the audio source
		 * by applying a gain modifier. The gain modifier represents a scaling factor,
		 * with a value of 1.0 being the original volume. Values less than 1.0 will decrease
		 * the volume, while values greater than 1.0 will increase the volume.
		 *
		 * @param[in] _value The gain modifier to set. Must be a positive value.
		 */
		inline void Gain(const ALfloat& _value) {
		    m_Parameters.m_GainModifier = std::max(_value, static_cast<ALfloat>(0.0));
			
			Sync();
		}
		
		/**
		 * @brief Getter function for the gain modifier of the audio source.
		 *
		 * This function returns the current gain modifier of the audio source. The gain modifier determines the volume at which the audio source is played.
		 *
		 * @return A constant reference to the gain modifier.
		 */
		[[nodiscard]] constexpr const ALfloat& Gain() const noexcept {
			return m_Parameters.m_GainModifier;
		}
		
		/**
		 * @brief Set the minimum gain value for the audio source.
		 *
		 * This function sets the minimum gain value for the audio source. The minimum gain
		 * value determines the lowest volume level at which the audio source can be played.
		 * The specified value is clamped to be between 0.0 and the current maximum gain value
		 * of the audio source.
		 *
		 * @param[in] _value The new minimum gain value for the audio source.
		 */
		inline void MinGain(const ALfloat& _value) {
		    m_Parameters.m_MinGain = std::max(_value, static_cast<ALfloat>(0.0));
		    m_Parameters.m_MaxGain = std::max(_value, m_Parameters.m_MaxGain);
			
			Sync();
		}
		
		/**
		 * @brief Get the minimum gain of the audio source.
		 *
		 * This function returns the minimum gain value of the audio source. The minimum gain
		 * represents the minimum amplification that can be applied to the audio data.
		 *
		 * @return const ALfloat& - The minimum gain value.
		 */
		[[nodiscard]] constexpr const ALfloat& MinGain() const noexcept {
			return m_Parameters.m_MinGain;
		}
		
		/**
		 * @brief Sets the maximum gain value for the audio source.
		 *
		 * This function is used to set the maximum gain value for the audio source. The maximum gain value determines the maximum volume at which the audio source can be played.
		 *
		 * @param[in] _value The maximum gain value to set.
		 */
		inline void MaxGain(const ALfloat& _value) {
		    m_Parameters.m_MaxGain = std::max(_value, m_Parameters.m_MinGain);
			
			Sync();
		}
		
		/**
		 * @brief Gets the maximum gain of the audio source.
		 *
		 * @return The maximum gain of the audio source.
		 */
		[[nodiscard]] constexpr const ALfloat& MaxGain() const noexcept {
			return m_Parameters.m_MaxGain;
		}
		
		/**
		 * @brief Sets the rolloff value for the audio source.
		 *
		 * This function sets the rolloff value for the audio source. The rolloff value
		 * determines how quickly the audio attenuates as the listener moves farther away
		 * from the audio source.
		 *
		 * @param[in] _value The rolloff value to set.
		 */
		inline void Rolloff(const ALfloat& _value) {
		    m_Parameters.m_Rolloff = _value;
			
			Sync();
		}
		
		/**
		 * @brief Get the rolloff value of the audio source.
		 *
		 * @return const ALfloat& The rolloff value.
		 */
		[[nodiscard]] constexpr const ALfloat& Rolloff() const noexcept {
			return m_Parameters.m_Rolloff;
		}
		
		/**
		 * @brief Sets the minimum angle of the audio source for playback of directional audio.
		 *
		 * @param[in] _value The value of the minimum angle. It must be between 0.0 and 360.0.
		 *
		 * @note The function also updates the maximum angle if the specified value is greater than the current maximum angle.
		 */
		inline void MinAngle(const ALfloat& _value) {
		    m_Parameters.m_MinAngle = std::clamp(_value, static_cast<ALfloat>(0.0), static_cast<ALfloat>(360.0));
			m_Parameters.m_MaxAngle = std::max(_value, m_Parameters.m_MaxAngle);
			
			Sync();
		}
		
		/**
		 * @brief Get the minimum angle of the audio source.
		 *
		 * @return A constant reference to the minimum angle of the audio source.
		 */
		[[nodiscard]] constexpr const ALfloat& MinAngle() const noexcept {
			return m_Parameters.m_MinAngle;
		}
		
		/**
		 * @brief Sets the maximum angle for audio source for playback of directional audio.
		 *
		 * @param[in] _value The maximum angle in degrees.
		 */
		inline void MaxAngle(const ALfloat& _value){
	        m_Parameters.m_MaxAngle = std::max(_value, m_Parameters.m_MinAngle);
			
			Sync();
		}
		
		/**
		 * @brief Gets the maximum angle of the audio source.
		 *
		 * @return A constant reference to the maximum angle of the audio source.
		 */
		[[nodiscard]] constexpr const ALfloat& MaxAngle() const noexcept {
			return m_Parameters.m_MaxAngle;
		}
		
		/**
		 * @brief Sets the playback position of the audio source.
		 *
		 * This function allows you to set the audio playback position of the audio source.
		 * The position is specified in seconds from the start of the audio file.
		 *
		 * @param[in] _value The desired playback position in seconds.
		 */
		inline void PlaybackPosition(const ALfloat& _value) const {
			alSourcef(m_Source, AL_SEC_OFFSET, _value);
		}
		
		/**
		 * @brief Get the current playback position of the audio source.
		 *
		 * This function returns the current playback position of the audio source in seconds.
		 * The position is calculated based on the current time since the audio started playing.
		 *
		 * @return The current playback position in seconds.
		 */
		[[nodiscard]] inline ALfloat PlaybackPosition() const {
			
			ALfloat result;
			
	        alGetSourcef(m_Source, AL_SEC_OFFSET, &result);
			
			return result;
		}
		
		/**
		 * @brief Sets the playback position of the AudioSource in samples (AL_SAMPLE_OFFSET) or bytes (AL_BYTE_OFFSET).
		 *
		 * This function sets the playback position of the AudioSource based on the specified parameter and value.
		 *
		 * @param[in] _param The parameter specifying the type of playback position to set. Must be either AL_SAMPLE_OFFSET or AL_BYTE_OFFSET.
		 * @param[in] _value The value indicating the playback position to set. The interpretation of this value depends on the specified parameter.
		 *
		 * @note This function does not validate the parameter and value inputs. It is the caller's responsibility to ensure that valid inputs are passed.
		 *
		 * @see AL_SAMPLE_OFFSET
		 * @see AL_BYTE_OFFSET
		 */
		inline void PlaybackPosition(const ALenum& _param, const int& _value) const {
			alSourcei(m_Source, _param, _value);
		}
		
		/**
		 * @brief Gets the playback position of the AudioSource.
		 *
		 * This method retrieves the playback position of the AudioSource.
		 * The position can be expressed in samples (AL_SAMPLE_OFFSET) or bytes (AL_BYTE_OFFSET).
		 *
		 * @param[in] _param The parameter specifying the type of playback position to retrieve.
		 * @return The playback position as an integer value.
		 */
		[[nodiscard]] inline int PlaybackPosition(const ALenum& _param) const {
			
			int result;
			
	        alGetSourcei(m_Source, _param, &result);
			
			return result;
		}
	};
	
} // LouiEriksson::Engine::Audio

#endif //FINALYEARPROJECT_AUDIOSOURCE_H