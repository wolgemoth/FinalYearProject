#ifndef GAME_FLYCAM_H
#define GAME_FLYCAM_H

#include "../include/engine_audio.h"
#include "../include/engine_core.h"

using namespace LouiEriksson::Engine;

namespace LouiEriksson::Game::Scripts {
	
	/**
	 * \class FlyCam
	 *
	 * \brief Free-fly camera.
	 *
	 * FlyCam is a class that represents a free-fly camera.
	 * It inherits from the Script class and provides functionality for controlling the camera's position and rotation.
	 */
	class FlyCam final : public Script {
	
	protected:
	
		/** @brief Camera of the FlyCam. */
		std::weak_ptr<Graphics::Camera> m_Camera;
	
		/** @brief Transform of the FlyCam. */
		std::weak_ptr<Transform> m_Transform;
		
		/** @brief AudioListener component. */
		std::weak_ptr<Audio::AudioListener> m_AudioListener;
		
		/** @brief AudioSource component for gun sounds. */
		std::weak_ptr<Audio::AudioSource> m_GunSound;
		
		/** @brief Motion vector of the FlyCam. */
		glm::vec3 m_Motion;
		
		/** @brief Speed of FlyCam along each axis. */
		float m_MoveSpeed;
		
		/** @brief Speed of FlyCam rotation. */
		float m_LookSpeed;
		
		/** @brief Current rotation of the flycam in degrees. */
		glm::vec3 m_Rotation;
		
		/** @inheritdoc */
		void Begin() override;
	
		/** @inheritdoc */
		void Tick() override;
		
		/** @brief Synchronise the camera's parameters with the ones defined in Settings. */
		void SyncCameraSettings() noexcept;
		
	public:
	
		explicit FlyCam(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		
		/** @inheritdoc */
		~FlyCam() override;
	
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(FlyCam); };
	};
	
} // LouiEriksson::Game::Scripts

#endif //GAME_FLYCAM_H