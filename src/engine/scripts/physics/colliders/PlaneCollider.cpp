#include "PlaneCollider.h"

#include "../../core/Debug.h"
#include "../../core/Transform.h"
#include "../../ecs/GameObject.h"
#include "../Collider.h"

#include <BulletCollision/CollisionShapes/btBoxShape.h>

#include <exception>
#include <memory>

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
				Debug::Log(e);
			}
		}
		
	}
	
} // LouiEriksson::Engine::Physics