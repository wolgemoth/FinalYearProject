#include "Plane.h"

#include "../../engine/scripts/core/File.h"
#include "../../engine/scripts/core/Resources.h"
#include "../../engine/scripts/core/Script.h"
#include "../../engine/scripts/core/Transform.h"
#include "../../engine/scripts/ecs/GameObject.h"
#include "../../engine/scripts/graphics/Renderer.h"
#include "../../engine/scripts/physics/colliders/PlaneCollider.h"
#include "../../engine/scripts/physics/Rigidbody.h"
#include "../../src/engine/scripts/physics/Collider.h"

#include <memory>
#include <string>

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson::Game {
	
	Plane::Plane(const std::shared_ptr<GameObject>& _parent) noexcept : Script(_parent) {}
	Plane::~Plane() = default;
	
	void Plane::Begin() {
		
		if (const auto s = Parent()->GetScene().lock()) {
		
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
			auto renderer = s->Attach(Parent()->AddComponent<Renderer>());
			if (renderer == nullptr) {
				renderer = Parent()->AddComponent<Renderer>();
			}
			
			renderer->SetMesh(m_Mesh);
			renderer->SetMaterial(m_Material);
			renderer->SetTransform(transform);
			
			// Get or add Collider.
			auto collider = s->Attach(Parent()->AddComponent<PlaneCollider>());
			if (collider == nullptr) {
				collider = Parent()->AddComponent<PlaneCollider>();
			}
			
			collider->SetTransform(transform);
			collider->SetType(Collider::Type::Plane);
			
			// Get or add rigidbody.
			auto rigidbody = s->Attach(Parent()->AddComponent<Rigidbody>());
			if (rigidbody == nullptr) {
				rigidbody = Parent()->AddComponent<Rigidbody>();
			}
			
			rigidbody->SetTransform(transform);
			rigidbody->SetCollider(collider);
			rigidbody->Kinematic(true);
			rigidbody->Gravity(false);
			
			collider->SetRigidbody(rigidbody);
		}
	}
	
	void Plane::Tick() {
	
	}
	
} // LouiEriksson::Game