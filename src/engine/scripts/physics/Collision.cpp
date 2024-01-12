#include "Collision.h"

#include <BulletCollision/NarrowPhaseCollision/btManifoldPoint.h>

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	Collision::Collision(const btManifoldPoint& _point, const int& _body) {
		
		// Get the contact point of the collision from the manifold:
		{
			const auto contactPoint = _body == 0 ?
					_point.getPositionWorldOnA() :
					_point.getPositionWorldOnB();
			
			m_ContactPoint = glm::vec3(contactPoint.x(), contactPoint.y(), contactPoint.z());
		}
		
		// Get the normal of the collision from the manifold:
		{
			const auto normal = _body == 0 ?
					-_point.m_normalWorldOnB :
					 _point.m_normalWorldOnB;
			
			m_Normal = glm::vec3(normal.x(), normal.y(), normal.z());
		}
		
		// Get the impulse of the collision from the manifold:
		m_Impulse = _point.getAppliedImpulse();
	}
	
	const glm::vec3& Collision::ContactPoint() const {
		return m_ContactPoint;
	}
	
	const glm::vec3& Collision::Normal() const {
		return m_Normal;
	}
	
	const float& Collision::Impulse() const {
		return m_Impulse;
	}
}