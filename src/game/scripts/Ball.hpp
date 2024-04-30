#ifndef GAME_BALL_HPP
#define GAME_BALL_HPP

#include "../include/engine_audio.hpp"
#include "../include/engine_core.hpp"
#include "../include/engine_graphics.hpp"
#include "../include/engine_physics.hpp"

using namespace LouiEriksson::Engine;

namespace LouiEriksson::Game::Scripts {
	
	/**
	 * @class Ball
	 * @brief Ball object with Rigidbody physics.
	 */
	class Ball final : public Script {
	
	public:
	
		explicit Ball(const std::weak_ptr<ECS::GameObject>& _parent) : Script(_parent),
			m_StartingPosition(0.0),
			m_Radius(0.0) {}
		
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Ball); };
	
	private:
		
		/** @brief AudioSource of the ball. */
		std::weak_ptr<Audio::AudioSource> m_AudioSource;
		
		/** @brief Position of the ball when it was first spawned. */
		glm::vec3 m_StartingPosition;
		
		/** @brief Radius of the ball. */
		float m_Radius;
		
	protected:
	
		/** @inheritdoc */
		void Begin() override {
			
			if (const auto p = Parent()) {
			if (const auto s = p->GetScene()) {
				
				// Get Transform.
				if (const auto t = p->GetComponent<Transform>()) {
					
					// Get starting position.
					m_StartingPosition = t->Position();
					
					// Get radius.
					m_Radius = std::max(t->Scale().x, std::max(t->Scale().y, t->Scale().z));
					
					// Add Renderer.
					const auto renderer = p->AddComponent<Graphics::Renderer>();
					renderer->SetMesh(Resources::Get<Graphics::Mesh>("sphere"));
					renderer->SetMaterial(Resources::Get<Graphics::Material>("sphere"));
					renderer->SetTransform(t);
				
					// Add Collider.
					const auto collider = p->AddComponent<Physics::SphereCollider>();
					collider->SetTransform(t);
					collider->SetType(Physics::Collider::Type::Sphere);
					collider->Radius(m_Radius);
					
					// Add Rigidbody.
					const auto rigidbody = p->AddComponent<Physics::Rigidbody>();
					rigidbody->SetTransform(t);
					rigidbody->SetCollider(collider);
					rigidbody->Mass(static_cast<btScalar>(4.0) * glm::pi<btScalar>() * (m_Radius * m_Radius)); // Equation for area of sphere. Courtesy of :https://www.omnicalculator.com/math/area-of-sphere
					rigidbody->Drag(0.005); // Courtesy of: https://www.engineeringtoolbox.com/drag-coefficient-d_627.html
					
					collider->SetRigidbody(rigidbody);
					
					// Add AudioSource and Clip.
					const auto as = p->AddComponent<Audio::AudioSource>();
					as->Clip(Resources::Get<Audio::AudioClip>("Hollow_Bass"));
	
					m_AudioSource = as;
				}
			}}
		}
	
		/** @inheritdoc */
		void FixedTick() override  {
		
			if (const auto p = Parent()) {
			if (const auto t = p->GetComponent<Transform>()) {
				
				// 'Reset' balls which fall beneath a certain height.
				if (t->Position().y <= -100.0) {
					
					if (const auto rb = p->GetComponent<Physics::Rigidbody>()) {
				
						// Reset position.
						rb->Position(m_StartingPosition);
						
						// Reset motion.
						rb->Velocity       (glm::vec3(0.0));
						rb->AngularVelocity(glm::vec3(0.0));
					}
				}
			}}
		}
		
		/** @inheritdoc */
		void OnCollision(const Physics::Collision& _collision) override {
			
			if (const auto  p = Parent()) {
			if (const auto rb = p->GetComponent<Physics::Rigidbody>()) {
				
				// Threshold impulse to play a sound.
				const auto impulse_threshold = rb->Mass() * 3.0;
				
				// Pitch settings:
				const auto pitch_multiplier         = 2.0; // Total pitch multiplier.
				const auto pitch_impulse_multiplier = 0.4; // Effect of impulse on pitch (inverse).
				const auto pitch_radius_multiplier  = 1.0; // Effect of ball radius on pitch (inverse).
				const auto pitch_min                = 0.7; // Minimum final pitch.
				const auto pitch_max                = 2.0; // Maximum final pitch.
				
				// Volume settings:
				const auto volume_multiplier       = 2.0; // Total volume multiplier.
				const auto volume_pitch_multiplier = 5.0; // Effect of pitch on volume (inverse).
				const auto volume_min              = 0.1; // Minimum final volume.
				const auto volume_max              = 2.0; // Maximum final volume.
				
				if (_collision.Impulse() >= impulse_threshold) {
	
					auto as = m_AudioSource.lock();
					
					if (as != nullptr) {
						
						// Stop the source.
						as->Stop();
						
						/*
						 * We modify the pitch inversely proportional to the parameters since
						 * we typically view small things as having a higher pitch and large
						 * things as having a lower pitch.
						 *
						 * Volume gets modified directly proportional to the parameters since
						 * we typically view small things as being quiet and big things being
						 * loud.
						 */
						
						// Modify the pitch:
						as->Pitch(
							static_cast<btScalar>(
								std::clamp(
									pitch_multiplier / (
										(m_Radius * pitch_radius_multiplier) *
										(_collision.Impulse() * pitch_impulse_multiplier)
									),
									pitch_min,
									pitch_max
								)
							)
						);
						
						// Modify the volume:
						as->Gain(
							static_cast<btScalar>(
								std::clamp(
									volume_multiplier / (as->Pitch() * volume_pitch_multiplier),
									volume_min,
									volume_max
								)
							)
						);
						
						// Play the source.
						as->Play();
					}
				}
			}}
		}
		
	};
	
} // LouiEriksson::Game::Scripts

#endif //GAME_BALL_HPP