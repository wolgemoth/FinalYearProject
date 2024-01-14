#ifndef FINALYEARPROJECT_BALL_H
#define FINALYEARPROJECT_BALL_H

#include "../../engine/scripts/audio/AudioSource.h"
#include "../../engine/scripts/core/Script.h"
#include "../../engine/scripts/ecs/GameObject.h"
#include "../../engine/scripts/graphics/Material.h"
#include "../../engine/scripts/graphics/Mesh.h"

#include <glm/ext/vector_float3.hpp>

#include <memory>

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson::Game {
	
	/// <summary>
	/// Ball object with Rigidbody physics.
	/// </summary>
	class Ball final : public Script {
	
	private:
		
		/// <summary> Mesh of the Ball. </summary>
		inline static std::shared_ptr<Graphics::Mesh> s_Mesh { nullptr };
		
		/// <summary> Material of the Ball. </summary>
		inline static std::weak_ptr<Graphics::Material> s_Material;
	
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
	
	};
	
} // LouiEriksson::Game

#endif //FINALYEARPROJECT_BALL_H