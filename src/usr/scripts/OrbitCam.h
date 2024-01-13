#ifndef FINALYEARPROJECT_ORBITCAM_H
#define FINALYEARPROJECT_ORBITCAM_H

#include "../../engine/scripts/core/Script.h"
#include "../../engine/scripts/core/Transform.h"
#include "../../engine/scripts/ecs/Scene.h"
#include "../../engine/scripts/graphics/Camera.h"

#include <glm/ext/vector_float3.hpp>

#include <memory>

// @Assessor: This class has changed significantly since it was submitted for 3DGP. Please mark it for GACP.

namespace LouiEriksson::Game {
	
	/// <summary>
	/// Camera that orbits a target.
	/// </summary>
	class OrbitCam final : public Script {
	
		friend class Scene;
	
	protected:
	
		/// <summary> Camera of the OrbitCam. </summary>
		std::weak_ptr<Camera> m_Camera;
	
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
	
	public:
	
		explicit OrbitCam(const std::shared_ptr<GameObject>& _parent);
		~OrbitCam() override;
		
		/// <summary> Get the Camera of the OrbitCam. </summary>
		std::shared_ptr<Camera> GetCamera();
		
		/// <summary> Get the Transform of the OrbitCam. </summary>
		std::shared_ptr<Transform> GetTransform();
	
	};
	
} // LouiEriksson::Game

#endif //FINALYEARPROJECT_ORBITCAM_H