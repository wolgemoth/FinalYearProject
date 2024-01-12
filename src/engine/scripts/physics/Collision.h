#ifndef FINALYEARPROJECT_COLLISION_H
#define FINALYEARPROJECT_COLLISION_H

#include <BulletCollision/NarrowPhaseCollision/btManifoldPoint.h>
#include <glm/ext/vector_float3.hpp>

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	/// <summary>
	/// Contains information describing a collision between a Rigidbody and a Collider.
	/// </summary>
	struct Collision {
	
	private:
	
		/// <summary> Contact point of the Collision. </summary>
		glm::vec3 m_ContactPoint;
	
		/// <summary> Normal of the Collision. </summary>
		glm::vec3 m_Normal;
	
		/// <summary> Impulse of the Collision. </summary>
		float m_Impulse;
	
	public:
	
		 Collision(const btManifoldPoint& _point, const int& _body);
		~Collision() = default;
	
		/// <summary> Get the contact point of the Collision. </summary>
		[[nodiscard]] const glm::vec3& ContactPoint() const;
	
		/// <summary> Get the normal of the Collision. </summary>
		[[nodiscard]] const glm::vec3& Normal() const;
	
		/// <summary> Get the impulse of the Collision. </summary>
		[[nodiscard]] const float& Impulse() const;
	};
}

#endif //FINALYEARPROJECT_COLLISION_H