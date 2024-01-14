#ifndef FINALYEARPROJECT_AUDIOLISTENER_H
#define FINALYEARPROJECT_AUDIOLISTENER_H

#include "../ecs/Component.h"

#include <glm/ext/vector_float3.hpp>

#include <memory>

namespace LouiEriksson::ECS {
	
	class GameObject;
	
} // LouiEriksson::ECS

namespace LouiEriksson::Audio {
	
	class AudioListener : public ECS::Component {
	
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
		
		 explicit AudioListener(const std::shared_ptr<ECS::GameObject>& _parent) noexcept;
		~AudioListener() override;
		
		/// <summary> Initialise the AudioListener. </summary>
		void Init();
	
		/// <summary> Updates the AudioListener every frame.</summary>
		void Tick();
		
		/// <summary> Set the master gain of the AudioListener. </summary>
		void Gain(const float& _value) noexcept;
		
		/// <summary> Get the master gain of the AudioListener. </summary>
		[[nodiscard]] const float& Gain() const noexcept;
		
	};
	
} // LouiEriksson::Audio

#endif //FINALYEARPROJECT_AUDIOLISTENER_H