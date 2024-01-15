#ifndef FINALYEARPROJECT_AUDIOSOURCE_H
#define FINALYEARPROJECT_AUDIOSOURCE_H

#include "../ecs/Component.h"
#include "../core/Script.h"

#include <AL/al.h>
#include <glm/ext/vector_float3.hpp>

#include <memory>

namespace LouiEriksson::Audio {
	
	class AudioClip;
	
	class AudioSource final : public Script {
	
		/*
		 * Please refer to OpenAL-Soft spec:
		 * https://github.com/kcat/openal-soft/wiki/Programmer%27s-Guide
		 */
		
	private:
		
		struct Parameters {
		
			/// <summary>
			/// Determines whether the AudioSource is global or not.
			/// Global AudioSources do not play positional audio.
			/// </summary>
			bool m_IsGlobal;
			
			/// <summary>
			/// Determines whether or not the AudioSource should loop the current clip.
			/// </summary>
			bool m_Loop;
			
			/// <summary>
			/// Panning of audio emitted from the AudioSource.
			/// Please note that this only applies if the source is global.
			/// </summary>
			glm::vec3 m_Panning;
			
			/// <summary>
			/// Minimum distance for this AudioSource.
			/// Must be a value greater than zero.
			/// If the distance between the listener and the AudioSource is below this level,
			/// audio will be emitted from this AudioSource at the maximum gain value.
			/// </summary>
			float m_MinDistance;
			
			/// <summary>
			/// Maximum distance for this AudioSource.
			/// Must be a value greater than or equal to the minimum distance.
			/// If the distance between the listener and the AudioSource is above this level,
			/// audio will be emitted from this AudioSource at the minimum gain value.
			/// </summary>
			float m_MaxDistance;
			
			/// <summary>
			/// Pitch multiplier of this AudioSource.
			/// Must be a value greater than zero.
			/// </summary>
			float m_Pitch;
			
			/// <summary>
			/// Gain modifier for this AudioSource.
			/// Must be a value greater than or equal to zero.
			/// </summary>
			float m_GainModifier;
			
			/// <summary>
			/// Minimum gain for this AudioSource.
			/// Must be a value greater than or equal to zero.
			/// </summary>
			float m_MinGain;
			
			/// <summary>
			/// Maximum gain for this AudioSource.
			/// Must be a value greater than or equal to the minimum gain.
			/// </summary>
			float m_MaxGain;
			
			/// <summary>
			/// Rolloff factor of this AudioSource.
			/// </summary>
			float m_Rolloff;
			
			/// <summary>
			/// Minimum angle of the AudioSource.
			/// Must be a value greater than or equal to zero.
			/// By default this value is 360 degrees, for omnidirectional playback.
			/// Values less than 360 degrees cause the AudioSource to play directionally.
			/// </summary>
			float m_MinAngle;
			
			/// <summary>
			/// Maximum angle of the AudioSource.
			/// Must be a value greater than or equal to the minimum angle.
			/// By default this value is 360 degrees.
			/// </summary>
			float m_MaxAngle;
			
			 Parameters() noexcept;
			~Parameters();
			
		};
		
		/// <summary>
		/// Identifier of this source as used by OpenAL.
		/// Please note that a value of 0u (or AL_NONE) indicates that
		/// the source has not yet initialised or initialised incorrectly.
		/// </summary>
		ALuint m_Source;
		
		Parameters m_Parameters;
		
		/// <summary> Weak reference to most recently played AudioClip. </summary>
		std::weak_ptr<AudioClip> m_Clip;
		
		/// <summary> Last position of the AudioSource (for computing transform-based velocity). </summary>
		glm::vec3 m_LastPosition;
		
		/// <summary> Synchronise the AudioSource with the internal audio engine. </summary>
		void Sync();
		
	public:
		
		 explicit AudioSource(const std::weak_ptr<ECS::GameObject>& _parent);
		~AudioSource() override;
		
		std::type_index TypeID() const noexcept override { return typeid(AudioSource); };
		
		/// <summary> Initialise the AudioSource.</summary>
		void Begin() override;
		
		/// <summary> Updates the AudioSource every frame.</summary>
		void Tick() override;
		
		/// <summary> Play this AudioSource.</summary>
		void Play(const bool& _allowFallback = true);
		
		/// <summary> Pause this AudioSource.</summary>
		void Pause() const;
		
		/// <summary> Stop this AudioSource.</summary>
		void Stop() const;
		
		/// <summary> Set this AudioSource's AudioClip. </summary>
		void Clip(const std::weak_ptr<AudioClip>& _value) noexcept;
		
		/// <summary> Get weak reference to the currently used AudioClip. </summary>
		[[nodiscard]] const std::weak_ptr<AudioClip>& Clip() const noexcept;
		
		/// <summary> Get the current state of the AudioSource. </summary>
		[[nodiscard]] ALenum State() const;
		
		/// <summary> Sets whether this AudioSource is global or not. </summary>
		void Global(const bool& _value);
		
		/// <summary> Gets whether this AudioSource is global or not. </summary>
		[[nodiscard]] const bool& Global() const noexcept;
		
		/// <summary> Sets whether this AudioSource should loop or not. </summary>
		void Loop(const bool& _value);
		
		/// <summary> Gets whether this AudioSource loops or not. </summary>
		[[nodiscard]] const bool& Loop() const noexcept;
		
		/// <summary> Sets the minimum distance for this AudioSource. </summary>
		void MinDistance(const float& _value);
		
		/// <summary> Gets the minimum distance for this AudioSource. </summary>
		[[nodiscard]] const float& MinDistance() const noexcept;
		
		/// <summary> Sets the maximum distance for this AudioSource. <summary>
		void MaxDistance(const float& _value);
		
		/// <summary> Gets the minimum distance for this AudioSource. </summary>
		[[nodiscard]] const float& MaxDistance() const noexcept;
		
		/// <summary> Sets the pitch multiplier of this AudioSource. <summary>
		void Pitch(const float& _value);
		
		/// <summary> Gets the pitch multiplier of this AudioSource. </summary>
		[[nodiscard]] const float& Pitch() const noexcept;
		
		/// <summary> Sets the gain modifier of this AudioSource. <summary>
		void Gain(const float& _value);
		
		/// <summary> Gets the gain modifier of this AudioSource. </summary>
		[[nodiscard]] const float& Gain() const noexcept;
		
		/// <summary> Sets the minimum gain of this AudioSource. <summary>
		void MinGain(const float& _value);
		
		/// <summary> Gets the minimum gain of this AudioSource. </summary>
		[[nodiscard]] const float& MinGain() const noexcept;
		
		/// <summary> Sets the maximum gain of this AudioSource. <summary>
		void MaxGain(const float& _value);
		
		/// <summary> Gets the maximum gain of this AudioSource. </summary>
		[[nodiscard]] const float& MaxGain() const noexcept;
		
		/// <summary> Sets the rolloff factor of this AudioSource. <summary>
		void Rolloff(const float& _value);
		
		/// <summary> Gets the rolloff factor of this AudioSource. </summary>
		[[nodiscard]] const float& Rolloff() const noexcept;
		
		/// <summary> Sets the minimum angle of this AudioSource. <summary>
		void MinAngle(const float& _value);
		
		/// <summary> Gets the minimum angle of this AudioSource. </summary>
		[[nodiscard]] const float& MinAngle() const noexcept;
		
		/// <summary> Sets the maximum angle of this AudioSource. <summary>
		void MaxAngle(const float& _value);
		
		/// <summary> Gets the maximum angle of this AudioSource. </summary>
		[[nodiscard]] const float& MaxAngle() const noexcept;
		
		/// <summary> Sets the playback position of the AudioSource in seconds. <summary>
		void PlaybackPosition(const float& _value) const;
		
		/// <summary> Gets the playback position of the AudioSource expressed in seconds. </summary>
		[[nodiscard]] float PlaybackPosition() const;
		
		/// <summary>
		/// Sets the playback position of the AudioSource in samples (AL_SAMPLE_OFFSET) or bytes (AL_BYTE_OFFSET).
		/// </summary>
		void PlaybackPosition(const ALenum& _param, const int& _value) const;
		
		/// <summary>
		/// Gets the playback position of the AudioSource expressed in samples (AL_SAMPLE_OFFSET) or bytes (AL_BYTE_OFFSET).
		/// </summary>
		[[nodiscard]] int PlaybackPosition(const ALenum& _param) const;
	};
	
} // LouiEriksson::Audio

#endif //FINALYEARPROJECT_AUDIOSOURCE_H