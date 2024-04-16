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
	
	/**
	 * @class AudioListener
	 * @brief Represents an audio listener in the scene.
	 *
	 * The AudioListener class is responsible for synchronizing the audio listener with the internal audio engine.
	 * It provides functionalities to set and value the master gain of the audio listener. The position, orientation,
	 * and velocity of the audio listener are automatically updated based on the parent GameObject's transform
	 * and rigidbody (if available).
	 *
	 * @see Please kindly refer to the <a href="https://github.com/kcat/openal-soft/wiki/Programmer%27s-Guide">OpenAL-Soft spec</a> for more information on OpenAL.
	 */
	class AudioListener final : public Script {
	
	private:
		
		/** @brief Master gain. Should be between zero and one. */
		float m_Gain;
		
		/** @brief Last position of the AudioListener (for computing transform-based velocity). */
		glm::vec3 m_LastPosition;
		
		/** @brief Synchronise the AudioListener with the internal audio engine. */
		void Sync();
		
	public:
		
		 explicit AudioListener(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		 
		/** @inheritdoc */
		~AudioListener() override;
		
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(AudioListener); };
		
		/** @brief Initialise the AudioListener. */
		void Begin() override;
	
		/** @brief Updates the AudioListener every frame. */
		void Tick() override;
		
		/**
		 * @brief Set the master gain of the AudioListener.
		 *
		 * This function sets the master gain of the AudioListener, which determines the volume of all audio played by the listener.
		 * @note: The _value parameter should be between 0.0f and 1.0f, with 0.0f being mute and 1.0f being full volume.
		 *
		 * @param[in] _value The value of the master gain to set.
		 */
		void Gain(const float& _value) noexcept;
		
		/**
		 * @brief Get the master gain of the AudioListener.
		 *
		 * This function returns the master gain of the AudioListener.
		 *
		 * @return The master gain value.
		 */
		[[nodiscard]] const float& Gain() const noexcept;
		
	};
	
} // LouiEriksson::Engine::Audio

#endif //FINALYEARPROJECT_AUDIOLISTENER_H