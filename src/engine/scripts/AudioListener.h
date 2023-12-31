#ifndef FINALYEARPROJECT_AUDIOLISTENER_H
#define FINALYEARPROJECT_AUDIOLISTENER_H

#include "Transform.h"

#include "Sound.h"

namespace LouiEriksson {
	
	class AudioListener: public Component {
	
	/*
	 * Please refer to OpenAL-Soft spec:
	 * https://github.com/kcat/openal-soft/wiki/Programmer%27s-Guide
	 */
	
	private:
		
		/// <summary> Master gain. Should be between zero and one. </summary>
		float m_Gain;
		
		/// <summary> Last position of the AudioListener (for computing transform-based velocity). </summary>
		glm::vec3 m_LastPosition;
		
		/// <summary> Synchronise the AudioListener with the internal audio engine. </summary>
		void Sync();
		
	public:
		
		 explicit AudioListener(const std::shared_ptr<GameObject>& _parent);
		~AudioListener() override;
		
		/// <summary> Initialise the AudioListener. </summary>
		void Init();
	
		/// <summary> Updates the AudioListener every frame.</summary>
		void Tick();
		
		/// <summary> Set the master gain of the AudioListener. </summary>
		void Gain(const float& _value);
		
		/// <summary> Get the master gain of the AudioListener. </summary>
		[[nodiscard]] const float& Gain() const;
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_AUDIOLISTENER_H