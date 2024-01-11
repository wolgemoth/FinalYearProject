#include "stdafx.h"

#include "Ball.h"

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	Ball::Ball(const std::shared_ptr<GameObject>& _parent) : Script(_parent) {
		m_StartingPosition = glm::vec3(0.0f);
		m_Radius = 0.0f;
	}
	
	Ball::~Ball() = default;
	
	void Ball::Begin() {
	
		auto scene = Parent()->GetScene();
	
		// Load mesh.
		if (s_Mesh == nullptr) {
			File::TryLoad("models/sphere/sphere.obj", s_Mesh);
		}
		
		if (s_Material.expired()) {
			
			// Create material from shader.
			s_Material = Resources::GetMaterial("sphere");
		}
	
		// Get or add component.
		auto transform = Parent()->GetComponent<Transform>();
		if (transform == nullptr) {
			transform = Parent()->AddComponent<Transform>();
		}
		
		// Get starting position.
		m_StartingPosition = transform->m_Position;
	
		// Get radius.
		m_Radius = glm::max(transform->m_Scale.x, glm::max(transform->m_Scale.y, transform->m_Scale.z));
	
		// Get or add renderer.
		auto renderer = scene->Attach(Parent()->AddComponent<Renderer>());
		if (renderer == nullptr) {
			renderer = Parent()->AddComponent<Renderer>();
		}
	
		renderer->SetMesh(s_Mesh);
		renderer->SetMaterial(s_Material);
		renderer->SetTransform(transform);
	
		// Get or add collider.
		auto collider = scene->Attach<SphereCollider>(Parent()->AddComponent<SphereCollider>());
		if (collider == nullptr) {
			collider = Parent()->AddComponent<SphereCollider>();
		}
	
		collider->SetTransform(transform);
		collider->SetType(Collider::Type::Sphere);
		collider->Radius(m_Radius);
	
		// Get or add rigidbody.
		auto rigidbody = scene->Attach(Parent()->AddComponent<Rigidbody>());
		if (rigidbody == nullptr) {
			rigidbody = Parent()->AddComponent<Rigidbody>();
		}
		
		rigidbody->SetTransform(transform);
		rigidbody->SetCollider(collider);
		rigidbody->Mass(4.0f * glm::pi<float>() * (m_Radius * m_Radius)); // Equation for area of sphere. Courtesy of :https://www.omnicalculator.com/math/area-of-sphere
		rigidbody->Drag(0.005f); // Courtesy of: https://www.engineeringtoolbox.com/drag-coefficient-d_627.html
	
		collider->SetRigidbody(rigidbody);
		
		// Get or add AudioSource.
		m_AudioSource = Parent()->AddComponent<AudioSource>();
		if (m_AudioSource.expired()) {
			m_AudioSource = Parent()->AddComponent<AudioSource>();
		}
		
		auto clip = Resources::GetAudio("Hollow_Bass");
		
		m_AudioSource.lock()->Clip(clip);
		m_AudioSource.lock()->Global(true);
	}
	
	void Ball::Tick() {
	
	}
	
	void Ball::FixedTick() {
	
		const auto transform = Parent()->GetComponent<Transform>();
		
		if (transform != nullptr) {
			
			// 'Reset' balls which fall beneath a certain height.
			if (transform->m_Position.y <= -100.0f) {
				
				const auto rb = Parent()->GetComponent<Rigidbody>();
		
				// Reset position.
				rb->Position(m_StartingPosition);
				
				// Reset motion.
				rb->Velocity       (glm::vec3(0.0f));
				rb->AngularVelocity(glm::vec3(0.0f));
			}
		}
	}
	
	void Ball::OnCollision(const Collision& _collision) {
		
		const auto rb = Parent()->GetComponent<Rigidbody>();
		
		if (rb != nullptr) {
			
			// Threshold impulse to play a sound.
			const auto impulse_threshold = rb->Mass() * 3.0f;
			
			// Pitch settings:
			const auto pitch_multiplier         = 2.0f; // Total pitch multiplier.
			const auto pitch_impulse_multiplier = 0.4f; // Effect of impulse on pitch (inverse).
			const auto pitch_radius_multiplier  = 1.0f; // Effect of ball radius on pitch (inverse).
			const auto pitch_min                = 0.7f; // Minimum final pitch.
			const auto pitch_max                = 2.0f; // Maximum final pitch.
			
			// Volume settings:
			const auto volume_multiplier       = 2.0f; // Total volume multiplier.
			const auto volume_pitch_multiplier = 5.0f; // Effect of pitch on volume (inverse).
			const auto volume_min              = 0.1f; // Minimum final volume.
			const auto volume_max              = 2.0f; // Maximum final volume.
			
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
						glm::clamp(
							pitch_multiplier / (
								(m_Radius * pitch_radius_multiplier) *
								(_collision.Impulse() * pitch_impulse_multiplier)
							),
							pitch_min,
							pitch_max
						)
					);
					
					// Modify the volume:
					as->Gain(
						glm::clamp(
							volume_multiplier / (as->Pitch() * volume_pitch_multiplier),
							volume_min,
							volume_max
						)
					);
					
					// Play the source.
					as->Play();
				}
			}
		}
	}
}