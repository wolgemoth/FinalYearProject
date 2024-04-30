#ifndef FINALYEARPROJECT_COLLISION_HPP
#define FINALYEARPROJECT_COLLISION_HPP

#include <BulletCollision/NarrowPhaseCollision/btManifoldPoint.h>

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
	
		Collision(const btManifoldPoint& _point, const int& _body) {
			
			// Get the contact point of the collision from the manifold:
			{
				const auto contactPoint = _body == 0 ?
						_point.getPositionWorldOnA() :
						_point.getPositionWorldOnB();
				
				m_ContactPoint = glm::vec<3, btScalar>(contactPoint.x(), contactPoint.y(), contactPoint.z());
			}
			
			// Get the normal of the collision from the manifold:
			{
				const auto normal = _body == 0 ?
						-_point.m_normalWorldOnB :
						 _point.m_normalWorldOnB;
				
				m_Normal = glm::vec<3, btScalar>(normal.x(), normal.y(), normal.z());
			}
			
			// Get the impulse of the collision from the manifold:
			m_Impulse = _point.getAppliedImpulse();
		}
		
		/**
		 * @brief Get the contact point of the Collision.
		 *
		 * This function returns the contact point of the collision. The contact point is a
		 * glm::vec3 value representing the point where the collision occurred.
		 *
		 * @return const glm::vec3& The contact point.
		 */
		[[nodiscard]] constexpr const glm::vec3& ContactPoint() const noexcept {
			return m_ContactPoint;
		}
		
		/**
		 * @brief Get the normal vector of the Collision.
		 *
		 * This function returns the normal vector of the collision. The normal vector is a
		 * glm::vec3 value representing the direction perpendicular to the collision surface.
		 *
		 * @return const glm::vec3& The normal vector.
		 */
		[[nodiscard]] constexpr const glm::vec3& Normal() const noexcept {
			return m_Normal;
		}
		
		/**
		 * @brief Get the impulse of the Collision.
		 *
		 * This function returns the impulse of the collision. The impulse is a measure of the change in momentum due to the collision.
		 *
		 * @return const float& The impulse of the collision.
		 */
		[[nodiscard]] constexpr const float& Impulse() const noexcept {
			return m_Impulse;
		}
		
	};
	
} // LouiEriksson::Engine::Physics

#endif //FINALYEARPROJECT_COLLISION_HPP