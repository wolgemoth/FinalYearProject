#pragma once

#ifndef FINALYEARPROJECT_ORBITCAM_H
#define FINALYEARPROJECT_ORBITCAM_H

#include "Scene.h"

namespace LouiEriksson {
	
	/// <summary>
	/// Camera that orbits a target.
	/// </summary>
	class OrbitCam final : public Script {
	
		friend class Scene;
	
	protected:
	
		/// <summary> Camera of the OrbitCam. </summary>
		std::weak_ptr<Camera>    m_Camera;
	
		/// <summary>
		/// Transform of the OrbitCam.
		/// </summary>
		std::weak_ptr<Transform> m_Transform;
		
		/// <summary> Target of the OrbitCam. </summary>
		glm::vec3 m_Target;
	
		/// <summary> Speed of orbit. </summary>
		float m_OrbitSpeed;
	
		/// <summary> Distance from target. </summary>
		float m_OrbitDistance;
	
		/// <summary> Called at the beginning of the first frame. </summary>
		void Begin() override;
	
		/// <summary> Called every frame. </summary>
		void Tick() override;
	
	public:
	
		explicit OrbitCam(std::weak_ptr<GameObject> _parent);
		~OrbitCam() override;
	
		/// <summary> Get the Camera of the OrbitCam. </summary>
		std::weak_ptr<Camera>    GetCamera();
	
		/// <summary> Get the Transform of the OrbitCam. </summary>
		std::weak_ptr<Transform> GetTransform();
	
		/// <summary> Set the target of the OrbitCam. </summary>
		void Target(glm::vec3 _target);
		
		/// <summary> Get the target of the OrbitCam. </summary>
		const glm::vec3& Target();
	
		/// <summary> Set the FOV of the OrbitCam. </summary>
		void FOV(const float& _fov);
	
		/// <summary> Get the FOV of the OrbitCam. </summary>
		const float& FOV();
	};
}

#endif //FINALYEARPROJECT_ORBITCAM_H