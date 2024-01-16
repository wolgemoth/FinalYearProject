#include "PlaneCollider.h"

#include "../../core/Transform.h"
#include "../../ecs/GameObject.h"
#include "../Collider.h"

#include <BulletCollision/CollisionShapes/btBoxShape.h>

#include <exception>
#include <iostream>
#include <memory>

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson::Engine::Physics {
	
	PlaneCollider::PlaneCollider(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : Collider(_parent) {}
	PlaneCollider::~PlaneCollider() = default;
	
	void PlaneCollider::SetTransform(const std::weak_ptr<Transform>& _transform) noexcept {
		
		Collider::SetTransform(_transform); // Call base function to assign new transform.
		
		// Lock the transform and check it is valid.
		if (const auto t = m_Transform.lock()) {
			
			const auto s = t->m_Scale;
			
			/* Use box-shaped collision instead of a plane-shape so that
			 * we can allow our plane to have edges, and not just be infinite. */
			try {
				m_CollisionShape.reset(
					new btBoxShape({ s.x / 2.0f, 0.0001f, s.z / 2.0f })
				);
			}
			catch (const std::exception& e) {
				std::cout << e.what() << '\n';
			}
		}
		
	}
	
} // LouiEriksson::Engine::Physics