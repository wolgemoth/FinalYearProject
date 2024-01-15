#ifndef GAME_BALL_H
#define GAME_BALL_H

#include "../include/engine_audio.h"
#include "../include/engine_core.h"

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson::Game {
	
	/// <summary>
	/// Ball object with Rigidbody physics.
	/// </summary>
	class Ball final : public Script {
	
	private:
		
		/// <summary> AudioSource component. </summary>
		std::weak_ptr<Audio::AudioSource> m_AudioSource;
		
		/// <summary> Position of the ball when it spawned. </summary>
		glm::vec3 m_StartingPosition;
		
		/// <summary> Radius of the ball. </summary>
		float m_Radius;
		
	protected:
	
		/// <inheritdoc/>
		void Begin() override;
	
		/// <inheritdoc/>
		void Tick() override;
	
		/// <inheritdoc/>
		void FixedTick() override;
		
		/// <inheritdoc/>
		void OnCollision(const Physics::Collision& _collision) override;
		
	public:
	
		explicit Ball(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		~Ball() override;
		
		std::type_index TypeID() const noexcept override { return typeid(Ball); };
	
	};
	
} // LouiEriksson::Game

#endif //GAME_BALL_H