#ifndef FINALYEARPROJECT_COLLIDER_HPP
#define FINALYEARPROJECT_COLLIDER_HPP

#include "../core/Transform.hpp"
#include "../ecs/Component.hpp"
#include "../ecs/GameObject.hpp"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>

#include <memory>

namespace LouiEriksson::Engine::Physics {
	
	class Rigidbody;
	
	/**
	 * @class Collider
	 * @brief Represents a Collider Component that can be attached to a GameObject.
	 */
	class Collider : public ECS::Component {
	
		friend Rigidbody;
		
	public:
		
		/**
		 * @enum Type
		 * @brief The type of the collider.
		 */
		enum Type : unsigned char {
			Sphere, /**< @brief Collider is a Sphere Collider. */
			Plane   /**< @brief Collider is a Plane Collider. */
		};
	
		explicit Collider(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : ECS::Component(_parent),
			m_Type() {}
		
		/**
		 * @brief Set the Transform of the Collider.
		 * @param[in] _transform The Transform to set.
		 */
		virtual void SetTransform(const std::weak_ptr<Transform>& _transform) noexcept {
			m_Transform = _transform;
		}
	
		/**
		 * @brief Retrieve the Transform associated with the Collider.
		 * @return A weak pointer to the Transform.
		 */
		[[nodiscard]] const std::weak_ptr<Transform>& GetTransform() const noexcept {
			return m_Transform;
		}
	
		/**
		 * @brief Set the Rigidbody of the Collider.
		 * @param[in] _rigidbody The Transform to set.
		 */
		void SetRigidbody(const std::weak_ptr<Rigidbody>& _rigidbody) noexcept {
			m_Rigidbody = _rigidbody;
		}
	
		/**
		 * @brief Retrieve the Rigidbody associated with the Collider.
		 * @return A weak pointer to the Transform.
		 */
		[[nodiscard]] constexpr const std::weak_ptr<Rigidbody>& GetRigidbody() const noexcept {
			return m_Rigidbody;
		}
		
		/**
		 * @brief Set the type of the Collider.
		 *
		 * This function is used to set the type of the Collider/
		 *
		 * @param[in] _type The type of the Collider to be set.
		 */
		void SetType(const Type& _type) noexcept {
			m_Type = _type;
		}
		
		/**
		 * @brief Retrieve the type of the Collider.
		 *
		 * This function is used to value the type of the Collider.
		 *
		 * @return A reference to the type of the Collider.
		 */
		[[nodiscard]] constexpr const Type& GetType() const noexcept {
			return m_Type;
		}
	
	protected:
		
		/** @brief Type of the collider. */
		Type m_Type;
	
		/** @brief (Bullet Physics Engine) Shape of collider. */
		std::shared_ptr<btCollisionShape> m_CollisionShape;
		
		/** @brief Transform associated with collider. */
		std::weak_ptr<Transform> m_Transform;
	
		/** @brief Rigidbody associated with collider. */
		std::weak_ptr<Rigidbody> m_Rigidbody;
		
	};
	
} // LouiEriksson::Engine::Physics

#endif //FINALYEARPROJECT_COLLIDER_HPP