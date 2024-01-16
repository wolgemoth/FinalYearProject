#ifndef GAME_ORBITCAM_H
#define GAME_ORBITCAM_H

#include "../include/engine_core.h"

// @Assessor: This class has changed significantly since it was submitted for 3DGP. Please mark it for GACP.

using namespace LouiEriksson::Engine;

namespace LouiEriksson::Game::Scripts {
	
	/// <summary>
	/// Camera that orbits a target.
	/// </summary>
	class OrbitCam final : public Script {
	
	protected:
	
		/// <summary> Camera of the OrbitCam. </summary>
		std::weak_ptr<Graphics::Camera> m_Camera;
	
		/// <summary>
		/// Transform of the OrbitCam.
		/// </summary>
		std::weak_ptr<Transform> m_Transform;
		
		/// <summary> Target of the OrbitCam. </summary>
		glm::vec3 m_Target;
		
		/// <summary> Progression of the animation. </summary>
		float m_AnimationProgress;
		
		/// <inheritdoc/>
		void Begin() override;
	
		/// <inheritdoc/>
		void Tick() override;
	
		/// <summary> Synchronise the camera's parameters with the ones defined in Settings. </summary>
		void SyncCameraSettings() noexcept;
		
	public:
	
		explicit OrbitCam(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		~OrbitCam() override;
		
		std::type_index TypeID() const noexcept override { return typeid(OrbitCam); };
	};
	
} // LouiEriksson::Game::Scripts

#endif //GAME_ORBITCAM_H