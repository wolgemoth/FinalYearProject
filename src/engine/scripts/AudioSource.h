#ifndef FINALYEARPROJECT_AUDIOSOURCE_H
#define FINALYEARPROJECT_AUDIOSOURCE_H

#include "Component.h"

#include "AudioClip.h"

namespace LouiEriksson {
	
	class AudioSource : public Component {
	
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
			
			 Parameters();
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
		
		 explicit AudioSource(const std::shared_ptr<GameObject>& _parent);
		~AudioSource() override;
		
		/// <summary> Initialise the AudioSource.</summary>
		void Init();
	
		/// <summary> Updates the AudioSource every frame.</summary>
		void Tick();
		
		/// <summary> Play a sound from this AudioSource.</summary>
		void Play(const std::weak_ptr<AudioClip>& _clip, const bool& _allowFallback = true);
		
		/// <summary> Sets whether this AudioSource is global or not. </summary>
		void Global(const bool& _value);
		
		/// <summary> Gets whether this AudioSource is global or not. </summary>
		[[nodiscard]] const bool& Global() const;
		
		/// <summary> Sets whether this AudioSource should loop or not. </summary>
		void Loop(const bool& _value);
		
		/// <summary> Gets whether this AudioSource loops or not. </summary>
		[[nodiscard]] const bool& Loop() const;
		
		/// <summary> Sets the minimum distance for this AudioSource. </summary>
		void MinDistance(const float& _value);
		
		/// <summary> Gets the minimum distance for this AudioSource. </summary>
		[[nodiscard]] const float& MinDistance() const;
		
		/// <summary> Sets the maximum distance for this AudioSource. <summary>
		void MaxDistance(const float& _value);
		
		/// <summary> Gets the minimum distance for this AudioSource. </summary>
		[[nodiscard]] const float& MaxDistance() const;
		
		/// <summary> Sets the pitch multiplier of this AudioSource. <summary>
		void Pitch(const float& _value);
		
		/// <summary> Gets the pitch multiplier of this AudioSource. </summary>
		[[nodiscard]] const float& Pitch() const;
		
		/// <summary> Sets the gain modifier of this AudioSource. <summary>
		void Gain(const float& _value);
		
		/// <summary> Gets the gain modifier of this AudioSource. </summary>
		[[nodiscard]] const float& Gain() const;
		
		/// <summary> Sets the minimum gain of this AudioSource. <summary>
		void MinGain(const float& _value);
		
		/// <summary> Gets the minimum gain of this AudioSource. </summary>
		[[nodiscard]] const float& MinGain() const;
		
		/// <summary> Sets the maximum gain of this AudioSource. <summary>
		void MaxGain(const float& _value);
		
		/// <summary> Gets the maximum gain of this AudioSource. </summary>
		[[nodiscard]] const float& MaxGain() const;
		
		/// <summary> Sets the rolloff factor of this AudioSource. <summary>
		void Rolloff(const float& _value);
		
		/// <summary> Gets the rolloff factor of this AudioSource. </summary>
		[[nodiscard]] const float& Rolloff() const;
		
		/// <summary> Sets the minimum angle of this AudioSource. <summary>
		void MinAngle(const float& _value);
		
		/// <summary> Gets the minimum angle of this AudioSource. </summary>
		[[nodiscard]] const float& MinAngle() const;
		
		/// <summary> Sets the maximum angle of this AudioSource. <summary>
		void MaxAngle(const float& _value);
		
		/// <summary> Gets the maximum angle of this AudioSource. </summary>
		[[nodiscard]] const float& MaxAngle() const;
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_AUDIOSOURCE_H