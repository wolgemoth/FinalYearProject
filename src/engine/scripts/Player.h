#ifndef FINALYEARPROJECT_PLAYER_H
#define FINALYEARPROJECT_PLAYER_H

#include "Scene.h"

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	class Player final : public Script {
	
		friend class Scene;
	
	private:
		
		/// <summary> Test model curuthers. </summary>
		std::weak_ptr<GameObject> m_Player;
		
		/// <summary> Player Camera. </summary>
		std::weak_ptr<GameObject> m_Camera;
		
		/// <summary> Floor planes. </summary>
		std::vector<std::weak_ptr<GameObject>> m_Planes;
		
		/// <summary> Obstacles. </summary>
		std::vector<std::weak_ptr<GameObject>> m_Obstacles;
		
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
		
		/// <summary> Called at the beginning of the first frame. </summary>
		void Begin() override;
		
		/// <summary> Called every frame. </summary>
		void Tick() override;
		
		void SpawnPlanes();
		
		void SpawnCamera();
		
		void SpawnPlayer();
		
		void SpawnBalls();
	
	public:
	
		 explicit Player(const std::shared_ptr<GameObject>& _parent);
		~Player() override;
	
	};
}

#endif //FINALYEARPROJECT_PLAYER_H