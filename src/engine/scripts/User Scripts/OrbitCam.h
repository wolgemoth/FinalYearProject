#ifndef FINALYEARPROJECT_ORBITCAM_H
#define FINALYEARPROJECT_ORBITCAM_H

#include "../ECS/Scene.h"

// @Assessor: This class has changed significantly since it was submitted for 3DGP. Please mark it for GACP.

namespace LouiEriksson {
	
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
}

#endif //FINALYEARPROJECT_ORBITCAM_H