#ifndef FINALYEARPROJECT_AUDIOLISTENER_H
#define FINALYEARPROJECT_AUDIOLISTENER_H

#include "../ecs/Component.h"
#include "../core/Script.h"

#include <glm/ext/vector_float3.hpp>

#include <memory>
#include <typeindex>

namespace LouiEriksson::Engine::ECS {
	
	class GameObject;
	
} // LouiEriksson::Engine::ECS

namespace LouiEriksson::Engine::Audio {
	
	class AudioListener final : public Script {
	
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
		
		 explicit AudioListener(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		~AudioListener() override;
		
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(AudioListener); };
		
		/// <summary> Initialise the AudioListener. </summary>
		void Begin() override;
	
		/// <summary> Updates the AudioListener every frame.</summary>
		void Tick() override;
		
		/// <summary> Set the master gain of the AudioListener. </summary>
		void Gain(const float& _value) noexcept;
		
		/// <summary> Get the master gain of the AudioListener. </summary>
		[[nodiscard]] const float& Gain() const noexcept;
		
	};
	
} // LouiEriksson::Engine::Audio

#endif //FINALYEARPROJECT_AUDIOLISTENER_H