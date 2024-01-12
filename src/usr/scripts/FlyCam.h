#ifndef FINALYEARPROJECT_FLYCAM_H
#define FINALYEARPROJECT_FLYCAM_H

#include "../../engine/scripts/audio/AudioListener.h"
#include "../../engine/scripts/audio/AudioSource.h"
#include "../../engine/scripts/core/Script.h"
#include "../../engine/scripts/core/Transform.h"
#include "../../engine/scripts/ecs/Scene.h"
#include "../../engine/scripts/graphics/Camera.h"

#include <glm/ext/vector_float3.hpp>

#include <memory>

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
		
		/// <summary> AudioListener component. </summary>
		std::weak_ptr<AudioListener> m_AudioListener;
		
		/// <summary> AudioSource component for gun sounds. </summary>
		std::weak_ptr<AudioSource> m_GunSound;
		
		/// <summary> Current motion vector of the FlyCam. </summary>
		glm::vec3 m_Motion;
		
		/// <summary> Speed of FlyCam along each axis. </summary>
		float m_MoveSpeed;
		
		/// <summary> Speed of FlyCam rotation. </summary>
		float m_LookSpeed;
		
		// Current rotation of the flycam in degrees.
		glm::vec3 m_Rotation;
		
		/// <inheritdoc/>
		void Begin() override;
	
		/// <inheritdoc/>
		void Tick() override;
	
		/// <summary> Synchronise the camera's parameters with the ones defined in Settings. </summary>
		void SyncCameraSettings();
		
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