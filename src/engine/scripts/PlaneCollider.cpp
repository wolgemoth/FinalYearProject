#include "stdafx.h"

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
			
			// Compute the normal using the transform.
			const auto normal = transform->UP;
			
			/* The 'plane' collision shape requires a normal, hence
			 * why it is initialised here when the transform is assigned. */
			m_CollisionShape.reset(
				new btStaticPlaneShape(
					btVector3(
						normal.x,
						normal.y,
						normal.z
					),
					0.0f
				)
			);
		}
		
	}
	
	
}