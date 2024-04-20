#ifndef FINALYEARPROJECT_SPHERECOLLIDER_H
#define FINALYEARPROJECT_SPHERECOLLIDER_H

#include "../Collider.h"

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
		
		explicit SphereCollider(const std::weak_ptr<ECS::GameObject>& _parent);
		
		/** @inheritdoc */
		~SphereCollider() override;
		
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(SphereCollider); };
		
		/**
		 * @brief Set the radius of the SphereCollider.
		 * @param[in] _radius The radius of the SphereCollider.
		 */
		void Radius(const btScalar& _radius);
		
		/**
		 * @brief Get the radius of the SphereCollider.
		 * @return The radius of the SphereCollider.
		 */
		[[nodiscard]] btScalar Radius() const;
		
	};
	
} // LouiEriksson::Engine::Physics

#endif //FINALYEARPROJECT_SPHERECOLLIDER_H