#include "../stdafx.h"

#include "Plane.h"

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	Plane::Plane(const std::shared_ptr<GameObject>& _parent) : Script(_parent) {}
	Plane::~Plane() = default;
	
	void Plane::Begin() {
		
		auto scene = Parent()->GetScene();
		
		// Load mesh.
		if (m_Mesh == nullptr) {
			File::TryLoad("models/woodfloor/woodfloor.obj", m_Mesh);
		}
		
		if (m_Material.expired()) {
			
			// Create material from shader.
			m_Material = Resources::GetMaterial("woodfloor");
		}
		
		// Get or add Transform.
		auto transform = Parent()->GetComponent<Transform>();
		if (transform == nullptr) {
			transform = Parent()->AddComponent<Transform>();
		}
		
		transform->m_Scale = glm::vec3(50.0f, 1.0f, 50.0f);
		
		// Get or add Renderer.
		auto renderer = scene->Attach(Parent()->AddComponent<Renderer>());
		if (renderer == nullptr) {
			renderer = Parent()->AddComponent<Renderer>();
		}
		
		renderer->SetMesh(m_Mesh);
		renderer->SetMaterial(m_Material);
		renderer->SetTransform(transform);
		
		// Get or add Collider.
		auto collider = scene->Attach(Parent()->AddComponent<PlaneCollider>());
		if (collider == nullptr) {
			collider = Parent()->AddComponent<PlaneCollider>();
		}
		
		collider->SetTransform(transform);
		collider->SetType(Collider::Type::Plane);
		
		// Get or add rigidbody.
		auto rigidbody = scene->Attach(Parent()->AddComponent<Rigidbody>());
		if (rigidbody == nullptr) {
			rigidbody = Parent()->AddComponent<Rigidbody>();
		}
		
		rigidbody->SetTransform(transform);
		rigidbody->SetCollider(collider);
		rigidbody->Kinematic(true);
		rigidbody->Gravity(false);
		
		collider->SetRigidbody(rigidbody);
	}
	
	void Plane::Tick() {
	
	}
}