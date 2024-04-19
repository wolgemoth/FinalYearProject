#ifndef FINALYEARPROJECT_RIGIDBODY_H
#define FINALYEARPROJECT_RIGIDBODY_H

#include "../ecs/Component.h"

#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float3.hpp>
#include <LinearMath/btMotionState.h>
#include <LinearMath/btVector3.h>

#include <memory>
#include <typeindex>
#include <vector>

namespace LouiEriksson::Engine {
	
	struct Transform;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Physics {
	
	class Collider;
	struct Collision;
	
	/**
	 * @class Rigidbody
	 * @brief Rigidbody Component implementing Rigidbody dynamics and Collision response behaviour.
	 *
	 * The Rigidbody class represents a component that implements Rigidbody dynamics and Collision response behaviour.
	 * It inherits from the Component class and provides functionality for managing the Rigidbody parameters and interactions.
	 */
	class Rigidbody final : public ECS::Component {
	
	private:
		
		struct Parameters;
		
		/**
		 * @class BulletRigidbody
		 * @brief Represents an internal Rigidbody component in the Bullet Physics Engine.
		 *
		 * This class represents an internal Rigidbody component in the Bullet Physics Engine.
		 */
		class BulletRigidbody final {
			
			friend Parameters;
			
		public:
			
			std::shared_ptr<btRigidBody>     m_Rigidbody; /**< @brief (Bullet Physics Engine) Internal Rigidbody component. */
			std::shared_ptr<btMotionState> m_MotionState; /**< @brief (Bullet Physics Engine) Rigidbody motion state. */
			
			 BulletRigidbody(const std::weak_ptr<Transform>& _transform, const std::weak_ptr<Collider>& _collider, const Parameters& _parameters);
			~BulletRigidbody();
		};
		
		/**
		 * @struct Parameters
		 * @brief Parameters of a Rigidbody.
		 */
		struct Parameters final {
		
			friend Rigidbody;
			
		private:
			
			/** @brief Wrapped implementation of Bullet's btRigidBody. */
			std::shared_ptr<BulletRigidbody> m_BulletRigidbody;
			
			bool m_Kinematic;  /** @brief Whether or not the Rigidbody is influenced by external forces.                */
			bool m_UseGravity; /** @brief Whether or not the Rigidbody is affected by gravity.                          */
			bool m_Continuous; /** @brief Whether or not the Rigidbody should use continuous collision detection (CCD). */
			
			float m_Mass;        /** @brief Mass of the Rigidbody.         */
			float m_Drag;        /** @brief Drag of the Rigidbody.         */
			float m_AngularDrag; /** @brief Angular drag of the Rigidbody. */
			float m_Friction;    /** @brief Friction of the Rigidbody.     */
			float m_Bounciness;  /** @brief Bounciness of the Rigidbody.   */
			
			btVector3 m_Inertia; /** @brief Resistance of object to changes in angular velocity. */
			
		public:
			
			Parameters() noexcept;
			
		};
		
		/** @brief Instance of this Rigidbody's parameters. */
		Parameters m_Parameters;
		
		std::weak_ptr<Transform> m_Transform; /** @brief Transform of the Rigidbody. */
		std::weak_ptr<Collider>   m_Collider; /** @brief Collider of the Rigidbody.*/
	
		/** @brief List of collisions for the current tick of the physics engine. */
		std::vector<Collision> m_Collisions;
		
		/**
		 * @brief Reinitialises the BulletRigidbody component of the Rigidbody.
		 *
		 * This method is used to reinitialise the BulletRigidbody component of the Rigidbody.
		 * It creates a new BulletRigidbody using the current transform, collider, and parameters.
		 */
		void BulletReinitialise();
		
	public:
	
		explicit Rigidbody(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		~Rigidbody() override = default;
	
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Rigidbody); };
		
		/**
		 * @brief Interpolates the position and rotation of the Rigidbody
		 *
		 * @note This function should be called once per frame to keep the transform in sync
		 * with the physics engine.
		 */
		void Interpolate();
		
		/**
		 * @brief Syncs the transform of the object with the physics engine.
		 *
		 * This function updates the position and orientation of the object's transform
		 * based on the data obtained from the physics engine. It also retrieves collision
		 * information and stores it in a list of collision objects.
		 *
		 * @note This function should be called once per frame to keep the transform in sync
		 * with the physics engine.
		 */
		void Sync();
		
		/**
		 * @fn const std::vector<Collision>& Rigidbody::Collisions() const noexcept
		 * @brief Retrieve the list of collisions for the current tick of the physics engine.
		 *
		 * This function returns a constant reference to the list of collisions for the current tick of the physics engine.
		 *
		 * @note The list of collisions is updated during the Sync() function call.
		 *
		 * @return const std::vector<Collision>& The reference to the list of collisions.
		 */
		[[nodiscard]] const std::vector<Collision>& Collisions() const noexcept;
		
		/**
		 * @brief Set the Transform of the Rigidbody.
		 * @param[in] _transform A weak pointer to the Transform to set.
		 */
		void SetTransform(const std::weak_ptr<Transform>& _transform);
		
		/**
		 * @brief Get the Transform of the Rigidbody.
		 * @return const std::weak_ptr<Transform>& The weak pointer to the Transform object.
		 */
		[[nodiscard]] const std::weak_ptr<Transform>& GetTransform() const noexcept;
		
		/**
		 * @brief Set the Collider of the Rigidbody.
		 * @param[in] _collider A weak pointer to the Collider to set.
		 */
		void SetCollider(const std::weak_ptr<Collider>& _collider);
		
		/**
		 * @brief Get the Collider of the Rigidbody.
		 * @returns A weak pointer to the Collider.
		 */
		[[nodiscard]] const std::weak_ptr<Collider>& GetCollider() const noexcept;
		
		/**
		 * @brief Manually set the position of the Rigidbody.
		 * @warning You shouldn't normally need to do this. See AddForce() instead.
		 *
		 * @see AddForce(const glm::vec3&, const glm::vec3&)
		 * @param[in] _value The new position to set.
		 */
		void Position(const glm::vec3& _value);
		
		/**
		 * @brief Get the position of the Rigidbody.
		 * @return const glm::vec3& The position of the Rigidbody.
		 */
		const glm::vec3& Position();
		
		/**
		 * @brief Manually set the rotation of the Rigidbody.
		 * @warning You shouldn't normally need to do this. See AddForce() instead.
		 *
		 * @see AddForce(const glm::vec3&, const glm::vec3&)
		 * @param[in] _value The new rotation to set.
		 */
		void Rotation(const glm::quat& _value);
		
		/**
		 * @brief Get the rotation of the Rigidbody.
		 * @return const glm::quat& The rotation of the Rigidbody.
		 */
		const glm::quat& Rotation();
		
		/**
		 * @brief Set the kinematic state of the Rigidbody.
		 *
		 * This method sets the kinematic state of the Rigidbody.
		 * When a Rigidbody is kinematic, it is not influenced by external forces.
		 *
		 * @param[in] _value The boolean value indicating the kinematic state.
		 */
		void Kinematic(const bool& _value);
		
		/**
		 * @brief Get the kinematic state of the Rigidbody.
		 *
		 * This method returns the current kinematic state of the Rigidbody.
		 * When a Rigidbody is kinematic, it is not influenced by external forces.
		 *
		 * @return const bool& The reference to the kinematic state. If true, the Rigidbody is kinematic and not influenced by external forces. If false, the Rigidbody is affected by external forces.
		 */
		[[nodiscard]] const bool& Kinematic() const noexcept;
		
		/**
		 * @brief Set whether gravity is applied to the rigidbody.
		 * @param[in] _value The new value for the gravity parameter. Set to true to enable gravity, or false to
		 * disable it.
		 */
		void Gravity(const bool& _value);
		
		/**
		 * @brief Get whether gravity is applied to the rigidbody.
		 * @return const bool& The value for the gravity parameter.
		 */
		[[nodiscard]] const bool& Gravity() const noexcept;
		
		/**
		 * @brief Manually set the velocity of the Rigidbody.
		 * @warning You shouldn't normally need to do this. See AddForce() instead.
		 *
		 * @see AddForce(const glm::vec3&, const glm::vec3&)
		 * @param[in] _value The new velocity to set.
		 */
		void Velocity(const glm::vec3& _value);
		
		/**
		 * @brief Get the velocity of the Rigidbody.
		 * @return The velocity of the Rigidbody as a glm::vec3.
		 */
		[[nodiscard]] glm::vec3 Velocity() const;
		
		/**
		 * @brief Manually set the angular velocity of the Rigidbody.
		 * @warning You shouldn't normally need to do this. See AddForce() instead.
		 *
		 * @see AddForce(const glm::vec3&, const glm::vec3&)
		 * @param[in] _value The new angular velocity to set.
		 */
		void AngularVelocity(const glm::vec3& _value);
		
		/**
		 * @brief Get the angular velocity of the Rigidbody.
		 * @return A glm::vec3 representing the angular velocity.
		 */
		[[nodiscard]] glm::vec3 AngularVelocity() const;
		
		/**
		 * @brief Add a force to the Rigidbody.
		 *
		 * This function applies a force to the Rigidbody.
		 * The force vector determines the magnitude and direction of the force, while the relativePosition vector determines the position where the force is applied.
		 * If no relative position is provided, it defaults to (0, 0, 0), which means the force is applied at the center of mass.
		 *
		 * @param[in] _value            The force vector to be applied.
		 * @param[in] _relativePosition (optional) The position where the force is applied.
		 */
		void AddForce(const glm::vec3& _value, const glm::vec3& _relativePosition = glm::vec3(0.0));
		
		/**
		 * @brief Get the force of the Rigidbody.
		 * @return The force of the Rigidbody as a glm::vec3.
		 */
		[[nodiscard]] glm::vec3 GetForce() const;
		
		/**
		 * @brief Set the mass of the Rigidbody.
		 *
		 * This function sets the mass of the Rigidbody to the specified value.
		 * Values smaller than 0.005 will be clamped.
		 *
		 * @param[in] _value The new mass value for the Rigidbody.
		 */
		void Mass(const float& _value);
		
		/**
		 * @brief Get the mass of the Rigidbody.
		 * @return The mass of the Rigidbody.
		 */
		[[nodiscard]] const float& Mass() const noexcept;
		
		/**
		 * @brief Set the drag of the Rigidbody.
		 * @param[in] _value The new drag value.
		 */
		void Drag(const float& _value);
		
		/**
		 * @brief Get the drag of the Rigidbody.
		 * @return The value of the drag as a const reference to a float.
		 */
		[[nodiscard]] const float& Drag() const noexcept;
		
		/**
		 * @brief Set the angular drag of the Rigidbody.
		 * @param[in] _value The value to set the angular drag to.
		 */
		void AngularDrag(const float& _value);
		
		/**
		 * @brief Get the angular drag of the Rigidbody.
		 * @return const float& The angular drag value.
		 */
		[[nodiscard]] const float& AngularDrag() const noexcept;
		
		/**
		 * @brief Set the friction of the Rigidbody.
		 *
		 * This function sets the friction value of the Rigidbody. Friction affects how easily the Rigidbody can slide
		 * across surfaces. Higher friction values make it harder to slide while lower values make it easier to slide.
		 *
		 * @param[in] _value The friction value to set for the Rigidbody.
		 *
		 * @note The friction value must be non-negative.
		 */
		void Friction(const float& _value);
		
		/**
		 * @brief Get the friction of the Rigidbody.
		 * @return The friction of the Rigidbody.
		 */
		[[nodiscard]] const float& Friction() const noexcept;
		
		/**
		 * @brief Set the bounciness of the Rigidbody.
		 *
		 * This function sets the bounciness value of the Rigidbody.
		 * The bounciness determines how much force is retained when the Rigidbody collides with other objects.
		 *
		 * @param[in] _value The bounciness value to be set. It should be a positive value.
		 */
		void Bounciness(const float& _value);
		
		/**
		 * @brief Get the bounciness of the Rigidbody.
		 * @return const float& Returns a reference to the bounciness value.
		 */
		[[nodiscard]] const float& Bounciness() const noexcept;
		
	};
	
} // LouiEriksson::Engine::Physics

#endif //FINALYEARPROJECT_RIGIDBODY_H