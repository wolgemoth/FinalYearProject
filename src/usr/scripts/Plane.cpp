#include "Plane.h"

#include "../../engine/scripts/core/File.h"
#include "../../engine/scripts/core/Resources.h"
#include "../../engine/scripts/core/Script.h"
#include "../../engine/scripts/core/Transform.h"
#include "../../engine/scripts/ecs/GameObject.h"
#include "../../engine/scripts/ecs/Scene.h"
#include "../../engine/scripts/graphics/Renderer.h"
#include "../../engine/scripts/physics/colliders/PlaneCollider.h"
#include "../../engine/scripts/physics/Rigidbody.h"
#include "../../src/engine/scripts/physics/Collider.h"
#include "../../src/engine/scripts/physics/Collision.h"

#include <memory>
#include <string>

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson::Game {
	
	Plane::Plane(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : Script(_parent) {}
	Plane::~Plane() = default;
	
	void Plane::Begin() {
		
		if (const auto p =      Parent().lock()) {
		if (const auto s = p->GetScene().lock()) {
		
			// Get Transform.
			if (const auto transform = p->GetComponent<Transform>().lock()) {
			
				transform->m_Scale = glm::vec3(50.0f, 1.0f, 50.0f);
			
				// Add Renderer.
				const auto renderer = p->AddComponent<Graphics::Renderer>().lock();
				renderer->SetMesh(Resources::GetMesh("woodfloor"));
				renderer->SetMaterial(Resources::GetMaterial("woodfloor"));
				renderer->SetTransform(transform);
				
				// Add Collider.
				const auto collider = p->AddComponent<Physics::PlaneCollider>().lock();
				collider->SetTransform(transform);
				collider->SetType(Physics::Collider::Type::Plane);
				
				// Add Rigidbody.
				const auto rigidbody = p->AddComponent<Physics::Rigidbody>().lock();
				rigidbody->SetTransform(transform);
				rigidbody->SetCollider(collider);
				rigidbody->Kinematic(true);
				rigidbody->Gravity(false);
				
				collider->SetRigidbody(rigidbody);
			}
		}}
	}
	
	void Plane::Tick() {
	
	}
	
} // LouiEriksson::Game