#include "Plane.h"

using namespace LouiEriksson::Engine;

namespace LouiEriksson::Game::Scripts {
	
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
				
				renderer->SetMesh(Resources::Get<Graphics::Mesh>("woodfloor"));
				renderer->SetMaterial(Resources::Get<Graphics::Material>("woodfloor"));
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
	
} // LouiEriksson::Game::Scripts