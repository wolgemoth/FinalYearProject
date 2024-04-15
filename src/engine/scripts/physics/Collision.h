#ifndef FINALYEARPROJECT_COLLISION_H
#define FINALYEARPROJECT_COLLISION_H

#include <BulletCollision/NarrowPhaseCollision/btManifoldPoint.h>
#include <glm/ext/vector_float3.hpp>

namespace LouiEriksson::Engine::Physics {
	
	/**
	 * struct Collision
	 * @brief Contains information describing a collision between a Rigidbody and a Collider.
	 */
	struct Collision final {
	
	private:
	
		glm::vec3 m_ContactPoint; /**< @brief Collision point. */
		glm::vec3       m_Normal; /**< @brief Collision normal. */
		float          m_Impulse; /**< @brief Collision impulse. */
	
	public:
	
		 Collision(const btManifoldPoint& _point, const int& _body);
		~Collision() = default;
		
		/**
		 * @brief Get the contact point of the Collision.
		 *
		 * This function returns the contact point of the collision. The contact point is a
		 * glm::vec3 value representing the point where the collision occurred.
		 *
		 * @return const glm::vec3& The contact point.
		 */
		[[nodiscard]] const glm::vec3& ContactPoint() const noexcept;
		
		/**
		 * @brief Get the normal vector of the Collision.
		 *
		 * This function returns the normal vector of the collision. The normal vector is a
		 * glm::vec3 value representing the direction perpendicular to the collision surface.
		 *
		 * @return const glm::vec3& The normal vector.
		 */
		[[nodiscard]] const glm::vec3& Normal() const noexcept;
		
		/**
		 * @brief Get the impulse of the Collision.
		 *
		 * This function returns the impulse of the collision. The impulse is a measure of the change in momentum due to the collision.
		 *
		 * @return const float& The impulse of the collision.
		 */
		[[nodiscard]] const float& Impulse() const noexcept;
		
	};
	
} // LouiEriksson::Engine::Physics

#endif //FINALYEARPROJECT_COLLISION_H