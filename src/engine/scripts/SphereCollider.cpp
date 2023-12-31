#include "stdafx.h"

#include "SphereCollider.h"

// @Assessor: This class was originally created for PFG but has since been modified. Please mark it for GEP.

namespace LouiEriksson {
	
	SphereCollider::SphereCollider(const std::shared_ptr<GameObject>& _parent) : Collider(_parent) {
		
		// Initialise the collision shape as a sphere.
		m_CollisionShape.reset(new btSphereShape(0.5f));
	}
	SphereCollider::~SphereCollider() = default;
	
	void SphereCollider::Radius(const float& _radius) {
		
		// Get derived class from collision shape (as sphere shape).
		const auto sphereCollider = std::static_pointer_cast<btSphereShape>(m_CollisionShape);
		
		// Set the collider's radius.
		sphereCollider->setUnscaledRadius(_radius);
	}
	float SphereCollider::Radius() const {
		
		// Get derived class from collision shape (as sphere shape).
		const auto sphereCollider = std::static_pointer_cast<btSphereShape>(m_CollisionShape);
		
		// Return the collider's radius.
		return sphereCollider->getRadius();
	}
}