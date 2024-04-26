#ifndef GAME_PLANE_HPP
#define GAME_PLANE_HPP

#include "../include/engine_core.hpp"
#include "../include/engine_graphics.hpp"
#include "../include/engine_physics.hpp"

using namespace LouiEriksson::Engine;

namespace LouiEriksson::Game::Scripts {
	
	/**
	 * @class Plane
	 * @brief A Plane with Collision.
	 */
	class Plane final : public Script {
	
	protected:
	
		/** @brief Material of the plane. */
		inline static std::weak_ptr<Graphics::Material> m_Material;
	
		/** @inheritdoc */
		void Begin() override {
		
			if (const auto p = Parent()) {
			if (const auto s = p->GetScene().lock()) {
			
				// Get Transform.
				if (const auto transform = p->GetComponent<Transform>()) {
				
					transform->Scale({50.0, 1.0, 50.0});
				
					// Add Renderer.
					const auto renderer = p->AddComponent<Graphics::Renderer>();
					
					renderer->SetMesh(Resources::Get<Graphics::Mesh>("woodfloor"));
					renderer->SetMaterial(Resources::Get<Graphics::Material>("woodfloor"));
					renderer->SetTransform(transform);
					
					// Add Collider.
					const auto collider = p->AddComponent<Physics::PlaneCollider>();
					collider->SetTransform(transform);
					collider->SetType(Physics::Collider::Type::Plane);
					
					// Add Rigidbody.
					const auto rigidbody = p->AddComponent<Physics::Rigidbody>();
					rigidbody->SetTransform(transform);
					rigidbody->SetCollider(collider);
					rigidbody->Kinematic(true);
					rigidbody->Gravity(false);
					
					collider->SetRigidbody(rigidbody);
				}
			}}
		}
	
	public:
	
		explicit Plane(const std::weak_ptr<ECS::GameObject>& _parent) : Script(_parent) {};
	
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Plane); };
	};
	
} // LouiEriksson::Game::Scripts

#endif //GAME_PLANE_HPP