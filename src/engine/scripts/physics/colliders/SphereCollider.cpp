#include "SphereCollider.h"

#include "../../ecs/GameObject.h"
#include "../../physics/Collider.h"

#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <LinearMath/btScalar.h>

#include <memory>

namespace LouiEriksson::Engine::Physics {
	
	SphereCollider::SphereCollider(const std::weak_ptr<ECS::GameObject>& _parent) : Collider(_parent) {
		
		// Initialise the collision shape as a sphere.
		m_CollisionShape = std::make_shared<btSphereShape>(0.5);
	}
	
	void SphereCollider::Radius(const btScalar& _radius) {
		
		// Get derived class from collision shape (as sphere shape).
		const auto sphereCollider = std::static_pointer_cast<btSphereShape>(m_CollisionShape);
		
		// Set the collider's radius.
		sphereCollider->setUnscaledRadius(_radius);
	}
	btScalar SphereCollider::Radius() const {
		
		// Get derived class from collision shape (as sphere shape).
		const auto sphereCollider = std::static_pointer_cast<btSphereShape>(m_CollisionShape);
		
		// Return the collider's radius.
		return sphereCollider->getRadius();
	}
	
} // LouiEriksson::Engine::Physics