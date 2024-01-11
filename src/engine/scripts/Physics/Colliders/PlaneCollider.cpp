#include "../../stdafx.h"

#include "PlaneCollider.h"

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	PlaneCollider::PlaneCollider(const std::shared_ptr<GameObject>& _parent) : Collider(_parent) {}
	PlaneCollider::~PlaneCollider() = default;
	
	void PlaneCollider::SetTransform(const std::weak_ptr<Transform>& _transform) {
		
		Collider::SetTransform(_transform); // Call base function to assign new transform.
		
		// Lock the transform and check it is valid.
		const auto transform = m_Transform.lock();
		
		if (transform != nullptr) {
			
			const auto s = transform->m_Scale;
			
			/* Use box-shaped collision instead of a plane-shape so that
			 * we can allow our plane to have edges, and not just be infinite. */
			m_CollisionShape.reset(
				new btBoxShape({ s.x / 2.0f, 0.0001f, s.z / 2.0f })
			);
		}
		
	}
	
	
}