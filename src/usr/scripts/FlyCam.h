#ifndef FINALYEARPROJECT_FLYCAM_H
#define FINALYEARPROJECT_FLYCAM_H

#include "../include/engine_audio.h"
#include "../include/engine_core.h"

// @Assessor: This class has changed significantly since it was submitted for 3DGP. Please mark it for GACP.

namespace LouiEriksson::Game {
	
	/// <summary>
	/// Free-fly camera.
	/// </summary>
	class FlyCam final : public Script {
	
	protected:
	
		/// <summary> Camera of the FlyCam. </summary>
		std::weak_ptr<Graphics::Camera> m_Camera;
	
		/// <summary> Transform of the FlyCam. </summary>
		std::weak_ptr<Transform> m_Transform;
		
		/// <summary> AudioListener component. </summary>
		std::weak_ptr<Audio::AudioListener> m_AudioListener;
		
		/// <summary> AudioSource component for gun sounds. </summary>
		std::weak_ptr<Audio::AudioSource> m_GunSound;
		
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
		void SyncCameraSettings() noexcept;
		
	public:
	
		explicit FlyCam(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		~FlyCam() override;
	
	};
	
} // LouiEriksson::Game

#endif //FINALYEARPROJECT_FLYCAM_H