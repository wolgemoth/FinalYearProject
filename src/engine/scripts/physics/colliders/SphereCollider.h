#ifndef FINALYEARPROJECT_SPHERECOLLIDER_H
#define FINALYEARPROJECT_SPHERECOLLIDER_H

#include "../../ecs/GameObject.h"
#include "../Collider.h"

#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <LinearMath/btScalar.h>

#include <memory>
#include <typeindex>

namespace LouiEriksson::Engine::ECS {
	
	class GameObject;
	
} // LouiEriksson::Engine::ECS

namespace LouiEriksson::Engine::Physics {
	
	/**
	 * @class SphereCollider
	 * @brief Represents a Sphere Collider Component that can be attached to a GameObject.
	 *
	 * SphereCollider is a concrete implementation of the abstract Collider class.
	 * It represents a spherical collider used for collision detection and physics simulation.
	 */
	class SphereCollider final : public Collider {
	
	public:
		
		explicit SphereCollider(const std::weak_ptr<ECS::GameObject>& _parent) : Collider(_parent) {
			
			// Initialise the collision shape as a sphere.
			m_CollisionShape = std::make_shared<btSphereShape>(0.5);
		}
	
		
		/** @inheritdoc */
		/** @inheritdoc */
		[[nodiscard]] inline std::type_index TypeID() const noexcept override { return typeid(SphereCollider); };
		
		/**
		 * @brief Set the radius of the SphereCollider.
		 * @param[in] _radius The radius of the SphereCollider.
		 */
		inline void Radius(const btScalar& _radius) {
			
			// Get derived class from collision shape (as sphere shape).
			const auto sphereCollider = std::static_pointer_cast<btSphereShape>(m_CollisionShape);
			
			// Set the collider's radius.
			sphereCollider->setUnscaledRadius(_radius);
		}
		
		/**
		 * @brief Get the radius of the SphereCollider.
		 * @return The radius of the SphereCollider.
		 */
		[[nodiscard]] inline btScalar Radius() const {
			
			// Get derived class from collision shape (as sphere shape).
			const auto sphereCollider = std::static_pointer_cast<btSphereShape>(m_CollisionShape);
			
			// Return the collider's radius.
			return sphereCollider->getRadius();
		}
		
	};
	
} // LouiEriksson::Engine::Physics

#endif //FINALYEARPROJECT_SPHERECOLLIDER_H