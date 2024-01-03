#include "stdafx.h"

#include "PlaneCollider.h"

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	PlaneCollider::PlaneCollider(const std::shared_ptr<GameObject>& _parent) : Collider(_parent) {
		
		const auto normal = m_Transform.lock()->UP;
		
		m_CollisionShape.reset(new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), 0.0f));
	}
	
	PlaneCollider::~PlaneCollider() = default;
	
}