#ifndef FINALYEARPROJECT_FLYCAM_H
#define FINALYEARPROJECT_FLYCAM_H

#include "Scene.h"

#include "Sound.h"

// @Assessor: This class has changed significantly since it was submitted for 3DGP. Please mark it for GACP.

namespace LouiEriksson {
	
	/// <summary>
	/// Free-fly camera.
	/// </summary>
	class FlyCam final : public Script {
	
		friend class Scene;
	
	protected:
	
		/// <summary> Camera of the FlyCam. </summary>
		std::weak_ptr<Camera> m_Camera;
	
		/// <summary> Transform of the FlyCam. </summary>
		std::weak_ptr<Transform> m_Transform;
		
		/// <summary> Current motion vector of the FlyCam. </summary>
		glm::vec3 m_Motion;
		
		/// <summary> Speed of FlyCam along each axis. </summary>
		float m_MoveSpeed;
		
		/// <summary> Speed of FlyCam rotation. </summary>
		float m_LookSpeed;
		
		// Current rotation of the flycam in degrees.
		glm::vec3 m_Rotation;
		
		/// <summary> Progression of the animation. </summary>
		float m_AnimationProgress;
		
		/// <summary> Called at the beginning of the first frame. </summary>
		void Begin() override;
	
		/// <summary> Called every frame. </summary>
		void Tick() override;
	
	public:
	
		explicit FlyCam(const std::shared_ptr<GameObject>& _parent);
		~FlyCam() override;
		
		/// <summary> Get the Camera of the FlyCam. </summary>
		std::shared_ptr<Camera> GetCamera();
		
		/// <summary> Get the Transform of the FlyCam. </summary>
		std::shared_ptr<Transform> GetTransform();
	
	};
}

#endif //FINALYEARPROJECT_FLYCAM_H