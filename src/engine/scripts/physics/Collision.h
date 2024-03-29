#ifndef FINALYEARPROJECT_COLLISION_H
#define FINALYEARPROJECT_COLLISION_H

#include <BulletCollision/NarrowPhaseCollision/btManifoldPoint.h>
#include <glm/ext/vector_float3.hpp>

namespace LouiEriksson::Engine::Physics {
	
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
		[[nodiscard]] const glm::vec3& ContactPoint() const noexcept;
	
		/// <summary> Get the normal of the Collision. </summary>
		[[nodiscard]] const glm::vec3& Normal() const noexcept;
	
		/// <summary> Get the impulse of the Collision. </summary>
		[[nodiscard]] const float& Impulse() const noexcept;
		
	};
	
} // LouiEriksson::Engine::Physics

#endif //FINALYEARPROJECT_COLLISION_H