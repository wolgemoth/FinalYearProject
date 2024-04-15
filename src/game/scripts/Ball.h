#ifndef GAME_BALL_H
#define GAME_BALL_H

#include "../include/engine_audio.h"
#include "../include/engine_core.h"

using namespace LouiEriksson::Engine;

namespace LouiEriksson::Game::Scripts {
	
	/**
	 * @class Ball
	 * @brief Ball object with Rigidbody physics.
	 */
	class Ball final : public Script {
	
	private:
		
		/** @brief AudioSource of the ball. */
		std::weak_ptr<Audio::AudioSource> m_AudioSource;
		
		/** @brief Position of the ball when it was first spawned. */
		glm::vec3 m_StartingPosition;
		
		/** @brief Radius of the ball. */
		float m_Radius;
		
	protected:
	
		/** @inheritdoc */
		void Begin() override;
	
		/** @inheritdoc */
		void Tick() override;
	
		/** @inheritdoc */
		void FixedTick() override;
		
		/** @inheritdoc */
		void OnCollision(const Physics::Collision& _collision) override;
		
	public:
	
		explicit Ball(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		
		/** @inheritdoc */
		~Ball() override;
		
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Ball); };
	
	};
	
} // LouiEriksson::Game::Scripts

#endif //GAME_BALL_H