#include "stdafx.h"

#include "Ball.h"

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	Ball::Ball(const std::shared_ptr<GameObject>& _parent) : Script(_parent) {
	
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
		
		m_StartingPosition = transform->m_Position;
	
		// Get radius.
		const float r = glm::length(transform->m_Scale) / 2.0f;
	
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
		collider->Radius(r);
	
		// Get or add rigidbody.
		auto rigidbody = scene->Attach(Parent()->AddComponent<Rigidbody>());
		if (rigidbody == nullptr) {
			rigidbody = Parent()->AddComponent<Rigidbody>();
		}
		
		rigidbody->SetTransform(transform);
		rigidbody->SetCollider(collider);
		rigidbody->Mass(4.0f * glm::pi<float>() * (r * r)); // Equation for area of sphere. Courtesy of :https://www.omnicalculator.com/math/area-of-sphere
		rigidbody->Drag(0.005f); // Courtesy of: https://www.engineeringtoolbox.com/drag-coefficient-d_627.html
	
		collider->SetRigidbody(rigidbody);
	}
	
	void Ball::Tick() {
	
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
}