#ifndef GAME_FLYCAM_H
#define GAME_FLYCAM_H

#include "../include/engine_audio.h"
#include "../include/engine_core.h"

using namespace LouiEriksson::Engine;

namespace LouiEriksson::Game::Scripts {
	
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
		
		/// <summary> SinceEpoch motion vector of the FlyCam. </summary>
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
	
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(FlyCam); };
	};
	
} // LouiEriksson::Game::Scripts

#endif //GAME_FLYCAM_H