#include "stdafx.h"

#include "Plane.h"

namespace LouiEriksson {
	
	Plane::Plane(const std::shared_ptr<GameObject>& _parent) : Script(_parent) {
	
	}
	
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
		
		// Get or add Renderer.
		auto renderer = scene->Attach(Parent()->AddComponent<Renderer>());
		if (renderer == nullptr) {
			renderer = Parent()->AddComponent<Renderer>();
		}
		
		renderer->SetMesh(m_Mesh);
		renderer->SetMaterial(m_Material);
		renderer->SetTransform(transform);
		
		// Get or add Collider.
		auto collider = scene->Attach(Parent()->AddComponent<Collider>());
		if (collider == nullptr) {
			collider = Parent()->AddComponent<Collider>();
		}
		
		collider->SetTransform(transform);
		collider->SetType(Collider::Type::Plane);
	}
	
	void Plane::Tick() {
	
	}
}