#ifndef FINALYEARPROJECT_PLAYER_H
#define FINALYEARPROJECT_PLAYER_H

#include "../../engine/scripts/core/Script.h"

#include <glm/ext/vector_float3.hpp>

#include <memory>
#include <vector>

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson::Game {
	
	class Player final : public Script {
	
	private:
		
		/// <summary> Test model curuthers. </summary>
		std::weak_ptr<ECS::GameObject> m_Player;
		
		/// <summary> Player Camera. </summary>
		std::weak_ptr<ECS::GameObject> m_Camera;
		
		/// <summary> Floor planes. </summary>
		std::vector<std::weak_ptr<ECS::GameObject>> m_Planes;
		
		/// <summary> Obstacles. </summary>
		std::vector<std::weak_ptr<ECS::GameObject>> m_Obstacles;
		
		int m_NumObstacles;
		
		float m_MinPlaneX;
		float m_MaxPlaneX;
		float m_PlayerSpeed;
		float m_CameraSpeed;
		float m_GameSpeed;
		
		float m_PlayerMoveFreedom;
		
		glm::vec3 m_TargetOffset;
		glm::vec3 m_CameraOffset;
	
	protected:
		
		/// <inheritdoc/>
		void Begin() override;
		
		/// <inheritdoc/>
		void Tick() override;
		
		void SpawnPlanes();
		
		void SpawnCamera();
		
		void SpawnPlayer();
		
		void SpawnBalls();
	
	public:
	
		 explicit Player(const std::shared_ptr<ECS::GameObject>& _parent);
		~Player() override;
	
	};
	
} // LouiEriksson::Game

#endif //FINALYEARPROJECT_PLAYER_H