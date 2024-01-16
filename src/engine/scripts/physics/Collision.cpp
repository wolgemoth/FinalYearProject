#include "Collision.h"

#include <BulletCollision/NarrowPhaseCollision/btManifoldPoint.h>

namespace LouiEriksson::Engine::Physics {
	
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
	
	const glm::vec3& Collision::ContactPoint() const noexcept {
		return m_ContactPoint;
	}
	
	const glm::vec3& Collision::Normal() const noexcept {
		return m_Normal;
	}
	
	const float& Collision::Impulse() const noexcept {
		return m_Impulse;
	}
	
} // LouiEriksson::Engine::Physics