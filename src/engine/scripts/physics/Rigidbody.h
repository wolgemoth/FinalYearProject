#ifndef FINALYEARPROJECT_RIGIDBODY_H
#define FINALYEARPROJECT_RIGIDBODY_H

#include "../core/Debug.h"
#include "../core/Time.h"
#include "../core/Transform.h"
#include "../core/Types.h"
#include "../ecs/Component.h"
#include "../ecs/GameObject.h"
#include "../physics/Collision.h"
#include "Collider.h"
#include "Physics.h"

#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>
#include <LinearMath/btScalar.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

#include <cmath>
#include <exception>
#include <memory>
#include <stdexcept>
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
			
			BulletRigidbody(const std::weak_ptr<Transform>& _transform, const std::weak_ptr<Collider>& _collider, const Parameters& _parameters) :
				m_MotionState(new btDefaultMotionState())
			{
				
				try {
					
					if (const auto c = _collider.lock()) {
						
						// Initialise the rigidbody.
						m_Rigidbody = std::make_shared<btRigidBody>(
							_parameters.m_Mass,
							m_MotionState.get(),
							c->m_CollisionShape.get(),
							_parameters.m_Inertia
						);
						
						// Assign the drag and angular drag coefficients.
						m_Rigidbody->setDamping(_parameters.m_Drag, _parameters.m_AngularDrag);
						
						// Assign the friction coefficient.
						m_Rigidbody->setFriction(_parameters.m_Friction);
						
						// Assign the restitution (bounciness) of the rigidbody.
						m_Rigidbody->setRestitution(_parameters.m_Bounciness);
						
						// Set the transform of the rigidbody.
						{
							const auto pos  = _transform.lock()->Position();
							const auto quat = _transform.lock()->Rotation();
							
							// Get the rigidbody's transform and assign the current position and rotation.
							auto t = m_Rigidbody->getWorldTransform();
							t.setOrigin  ({ pos.x, pos.y, pos.z });
							t.setRotation({ quat.x, quat.y, quat.z, quat.w });
							
							// Update the rigidbody's transform with the new values.
							m_Rigidbody->setWorldTransform(t);
						}
						
						if (_parameters.m_Kinematic) {
							
							// Set the collision flags to kinematic, if specified.
							m_Rigidbody->setCollisionFlags(
								(unsigned)(m_Rigidbody->getCollisionFlags()) |
								(unsigned)btCollisionObject::CF_KINEMATIC_OBJECT
							);
							
							// Restrict all motion.
							m_Rigidbody-> setLinearFactor({ 0.0, 0.0, 0.0 });
							m_Rigidbody->setAngularFactor({ 0.0, 0.0, 0.0 });
						}
						else {
							
							// Ensure motion is permitted along all axes.
							m_Rigidbody-> setLinearFactor({ 1.0, 1.0, 1.0 });
							m_Rigidbody->setAngularFactor({ 1.0, 1.0, 1.0 });
						}
						
						// Set up how the rigidbody interacts with gravity:
						{
							const auto g = Physics::Gravity();
							
							m_Rigidbody->setGravity(
								_parameters.m_UseGravity ?
									btVector3( g.x,  g.y,  g.z) :
									btVector3(0.0, 0.0, 0.0)
							);
						}
						
						// See: https://docs.panda3d.org/1.10/python/programming/physics/bullet/ccd
						if (_parameters.m_Continuous) {
							
							// Compute the continuous sphere radius using the collider's AABB.
							btScalar sweep_sphere_radius = 0.01;
							{
								const auto* const col = m_Rigidbody->getCollisionShape();
							
								const btTransform t;
								btVector3 min;
								btVector3 max;
								
								col->getAabb(t, min, max);
								
								const auto delta = max - min;
								
								// Get largest axis:
								const btScalar multiplier = 0.707;
							
								sweep_sphere_radius = std::max(
									sweep_sphere_radius,
									std::max(
										delta.x(),
										std::max(
											delta.y(),
											delta.z()
										)
									) * multiplier
								);
							}
							
							// Threshold to activate CCD in units per second.
							const btScalar threshold_Ms = sweep_sphere_radius;
							
							m_Rigidbody->setCcdMotionThreshold(Time::FixedDeltaTime<btScalar>() * threshold_Ms);
							m_Rigidbody->setCcdSweptSphereRadius(sweep_sphere_radius);
						}
						
						// Finally, add the rigidbody to the dynamics world.
						Physics::s_DynamicsWorld->addRigidBody(m_Rigidbody.get());
						
					}
					else {
						Debug::Log("Couldn't lock _collider!", LogType::Error);
					}
				}
				catch (const std::exception& e) {
					Debug::Log(e);
				}
			}
			
			~BulletRigidbody()  {
			
				if (m_Rigidbody != nullptr) {
					Physics::s_DynamicsWorld->removeRigidBody(m_Rigidbody.get());
				}
			}
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
			
			btScalar m_Mass;        /** @brief Mass of the Rigidbody.         */
			btScalar m_Drag;        /** @brief Drag of the Rigidbody.         */
			btScalar m_AngularDrag; /** @brief Angular drag of the Rigidbody. */
			btScalar m_Friction;    /** @brief Friction of the Rigidbody.     */
			btScalar m_Bounciness;  /** @brief Bounciness of the Rigidbody.   */
			
			btVector3 m_Inertia; /** @brief Resistance of object to changes in angular velocity. */
			
		public:
			
			Parameters() noexcept :
				m_Kinematic (false),
				m_UseGravity(true),
				m_Continuous(true),
				
				m_Mass       (1.0),
				m_Drag       (0.2),
				m_AngularDrag(0.005),
				m_Friction   (0.5),
				m_Bounciness (0.5),
				
				m_Inertia(1.0, 1.0, 1.0) {}
			
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
		void BulletReinitialise() {
			
			try {
				if (const auto transform = m_Transform.lock()) {
				if (const auto  collider =  m_Collider.lock()) {
					
					m_Parameters.m_BulletRigidbody = std::make_shared<BulletRigidbody>(
						m_Transform,
						m_Collider,
						m_Parameters
					);
				}}
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
	}
		
	public:
		
		explicit Rigidbody(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : ECS::Component(_parent) {}
	
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Rigidbody); };
		
		/**
		 * @brief Interpolates the position and rotation of the Rigidbody
		 *
		 * @note This function should be called once per frame to keep the transform in sync
		 * with the physics engine.
		 */
		void Interpolate() {
			
			if (Physics::s_LastTick > 0.0) {
				
				if (const auto transform = m_Transform.lock()) {
					
					// Get the transform from the bullet physics engine.
					auto t = m_Parameters.m_BulletRigidbody->m_Rigidbody->getWorldTransform();
					
					// Get the position and rotation of the rigidbody as they exist within bullet.
					const auto bOrigin   = t.getOrigin();
					const auto bRotation = t.getRotation().inverse();
					
					// Convert to glm types.
					const auto lastPos = glm::vec3(  bOrigin.x(),   bOrigin.y(),   bOrigin.z());
					const auto lastRot = glm::quat(bRotation.w(), bRotation.x(), bRotation.y(), bRotation.z());
				
					// Compute quaternion from angular velocity.
					const auto av = AngularVelocity();
					
					glm::quat rot;
					if (glm::length(av) > 0.005) {
						rot = glm::angleAxis(glm::cos(glm::length(av)) * Time::FixedDeltaTime<scalar_t>(), glm::normalize(av));
					}
					else {
						rot = glm::identity<glm::quat>();
					}
					
					// Apply new values. Linearly interpolate using length of time since last physics update.
					transform->Position(lastPos + (Velocity() * static_cast<scalar_t>(Physics::s_LastTick * Time::FixedDeltaTime())));
					transform->Rotation(glm::slerp(lastRot, lastRot * rot, static_cast<scalar_t>(std::max(Physics::s_LastTick, static_cast<tick_t>(0.0)))));
				}
			}
		}
		
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
		void Sync() {
			
			try {
				
				/* SYNCHRONISE POSITION AND ORIENTATION */
				{
					if (const auto transform = m_Transform.lock()) {
						
						// Get the transform from the bullet physics engine.
						auto t = m_Parameters.m_BulletRigidbody->m_Rigidbody->getWorldTransform();
						
						const auto bOrigin   = t.getOrigin();
						const auto bRotation = t.getRotation().inverse();
						
						// Sync the transform from bullet with the transform in-engine.
						transform->Position(glm::vec3(  bOrigin.x(),   bOrigin.y(),   bOrigin.z()));
						transform->Rotation(glm::quat(bRotation.w(), bRotation.x(), bRotation.y(), bRotation.z()));
					}
				}
				
				/* GET COLLISIONS */
				{
					// Reset the collisions.
					m_Collisions.clear();
					
					/*
					 * TODO: Move this to the Physics class and have some kind of managed, internal
					 * system for indexing Rigidbody objects against btRigidbody objects.
					 */
					
					// Get and iterate through every contact manifold in the physics engine.
				    const auto numManifolds = Physics::s_DynamicsWorld->getDispatcher()->getNumManifolds();
				    for (auto i = 0; i < numManifolds; ++i) {
						
				        const auto* const contactManifold = Physics::s_DynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
				
				        // Get and iterate through every contact in the manifold.
				        const auto numContacts = contactManifold->getNumContacts();
				        for (auto j = 0; j < numContacts; j++) {
							
							// Get a pointer to the btRigidbody associated with this rigidbody.
							const auto* const rbThis = m_Parameters.m_BulletRigidbody->m_Rigidbody.get();
							
							// Validate the pointer. If is fails the null check we will throw.
							if (rbThis != nullptr) {
								
								// Determine if either of the bodies in the manifold are this rigidbody.
								int body;
								
								if      (contactManifold->getBody0() == rbThis) { body =  0; }
								else if (contactManifold->getBody1() == rbThis) { body =  1; }
								else                                            { body = -1; }
								
								// If one of the rigidbodies involved in the collision event are this rigidbody,
								// generate a collision object and append it to the list of collisions.
								if (body >= 0) {
									m_Collisions.emplace_back(contactManifold->getContactPoint(j), body);
								}
							}
							else {
								throw std::runtime_error("btRigidbody associated with this Rigidbody is nullptr!");
							}
				        }
					}
				}
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
		}
		
		/**
		 * @fn const std::vector<Collision>& Collisions() const noexcept
		 * @brief Retrieve the list of collisions for the current tick of the physics engine.
		 *
		 * This function returns a constant reference to the list of collisions for the current tick of the physics engine.
		 *
		 * @note The list of collisions is updated during the Sync() function call.
		 *
		 * @return const std::vector<Collision>& The reference to the list of collisions.
		 */
		[[nodiscard]] constexpr const std::vector<Collision>& Collisions() const noexcept {
			return m_Collisions;
		}
		
		/**
		 * @brief Set the Transform of the Rigidbody.
		 * @param[in] _transform A weak pointer to the Transform to set.
		 */
		inline void SetTransform(const std::weak_ptr<Transform>& _transform) {
			m_Transform = _transform;
			
			BulletReinitialise();
		}
		
		/**
		 * @brief Get the Transform of the Rigidbody.
		 * @return const std::weak_ptr<Transform>& The weak pointer to the Transform object.
		 */
		[[nodiscard]] constexpr const std::weak_ptr<Transform>& GetTransform() const noexcept {
			return m_Transform;
		}
		
		/**
		 * @brief Set the Collider of the Rigidbody.
		 * @param[in] _collider A weak pointer to the Collider to set.
		 */
		inline void SetCollider(const std::weak_ptr<Collider>& _collider) {
			
			m_Collider = _collider;
			
			if (const auto col = m_Collider.lock()) {
				col->m_CollisionShape->calculateLocalInertia(m_Parameters.m_Mass, m_Parameters.m_Inertia);
			}
			
			BulletReinitialise();
		}
		
		/**
		 * @brief Get the Collider of the Rigidbody.
		 * @returns A weak pointer to the Collider.
		 */
		[[nodiscard]] constexpr const std::weak_ptr<Collider>& GetCollider() const noexcept {
			return m_Collider;
		}
		
		/**
		 * @brief Manually set the position of the Rigidbody.
		 * @warning You shouldn't normally need to do this. See AddForce() instead.
		 *
		 * @see AddForce(const glm::vec3&, const glm::vec3&)
		 * @param[in] _value The new position to set.
		 */
		inline void Position(const glm::vec3& _value) {
			
			m_Transform.lock()->Position(_value);
			
			BulletReinitialise();
		}
		
		/**
		 * @brief Get the position of the Rigidbody.
		 * @return const glm::vec3& The position of the Rigidbody.
		 */
		[[nodiscard]] inline const glm::vec3& Position() {
			
			Sync();
			
			return m_Transform.lock()->Position();
		}
		
		/**
		 * @brief Manually set the rotation of the Rigidbody.
		 * @warning You shouldn't normally need to do this. See AddForce() instead.
		 *
		 * @see AddForce(const glm::vec3&, const glm::vec3&)
		 * @param[in] _value The new rotation to set.
		 */
		inline void Rotation(const glm::quat& _value) {
			
			m_Transform.lock()->Rotation(_value);
			
			BulletReinitialise();
		}
		
		/**
		 * @brief Get the rotation of the Rigidbody.
		 * @return const glm::quat& The rotation of the Rigidbody.
		 */
		[[nodiscard]] inline const glm::quat& Rotation() {
			
			Sync();
			
			return m_Transform.lock()->Rotation();
		}
		
		/**
		 * @brief Set the kinematic state of the Rigidbody.
		 *
		 * This method sets the kinematic state of the Rigidbody.
		 * When a Rigidbody is kinematic, it is not influenced by external forces.
		 *
		 * @param[in] _value The boolean value indicating the kinematic state.
		 */
		inline void Kinematic(const bool& _value) {
			m_Parameters.m_Kinematic = _value;
			
			BulletReinitialise();
		}
		
		/**
		 * @brief Get the kinematic state of the Rigidbody.
		 *
		 * This method returns the current kinematic state of the Rigidbody.
		 * When a Rigidbody is kinematic, it is not influenced by external forces.
		 *
		 * @return const bool& The reference to the kinematic state. If true, the Rigidbody is kinematic and not influenced by external forces. If false, the Rigidbody is affected by external forces.
		 */
		[[nodiscard]] constexpr const bool& Kinematic() const noexcept {
			return m_Parameters.m_Kinematic;
		}
		
		/**
		 * @brief Set whether gravity is applied to the rigidbody.
		 * @param[in] _value The new value for the gravity parameter. Set to true to enable gravity, or false to
		 * disable it.
		 */
		inline void Gravity(const bool& _value) {
			m_Parameters.m_UseGravity = _value;
			
			BulletReinitialise();
		}
		
		/**
		 * @brief Get whether gravity is applied to the rigidbody.
		 * @return const bool& The value for the gravity parameter.
		 */
		[[nodiscard]] constexpr const bool& Gravity() const noexcept {
			return m_Parameters.m_UseGravity;
		}
		
		/**
		 * @brief Manually set the velocity of the Rigidbody.
		 * @warning You shouldn't normally need to do this. See AddForce() instead.
		 *
		 * @see AddForce(const glm::vec3&, const glm::vec3&)
		 * @param[in] _value The new velocity to set.
		 */
		void Velocity(const glm::vec3& _value) {
			
			m_Parameters.m_BulletRigidbody->m_Rigidbody->setLinearVelocity(
				{ _value.x, _value.y, _value.z }
			);
		}
		
		/**
		 * @brief Get the velocity of the Rigidbody.
		 * @return The velocity of the Rigidbody as a glm::vec3.
		 */
		[[nodiscard]] inline glm::vec3 Velocity() const {
			
			const auto v = m_Parameters.m_BulletRigidbody->m_Rigidbody->getLinearVelocity();
			
			return {v.x(), v.y(), v.z()};
		}
		
		/**
		 * @brief Manually set the angular velocity of the Rigidbody.
		 * @warning You shouldn't normally need to do this. See AddForce() instead.
		 *
		 * @see AddForce(const glm::vec3&, const glm::vec3&)
		 * @param[in] _value The new angular velocity to set.
		 */
		void AngularVelocity(const glm::vec3& _value) {
			
			m_Parameters.m_BulletRigidbody->m_Rigidbody->setAngularVelocity(
				{ _value.x, _value.y, _value.z }
			);
		}
		
		/**
		 * @brief Get the angular velocity of the Rigidbody.
		 * @return A glm::vec3 representing the angular velocity.
		 */
		[[nodiscard]] inline glm::vec3 AngularVelocity() const {
			
			const auto av = m_Parameters.m_BulletRigidbody->m_Rigidbody->getAngularVelocity();
			
			return { av.x(), av.y(), av.z() };
		}
		
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
		void AddForce(const glm::vec3& _value, const glm::vec3& _relativePosition) {
		
			m_Parameters.m_BulletRigidbody->m_Rigidbody->applyForce(
				{            _value.x,            _value.y,            _value.z },
				{ _relativePosition.x, _relativePosition.y, _relativePosition.z }
			);
		}
		
		/**
		 * @brief Get the force of the Rigidbody.
		 * @return The force of the Rigidbody as a glm::vec3.
		 */
		[[nodiscard]] inline glm::vec3 GetForce() const {
			
			const auto f = m_Parameters.m_BulletRigidbody->m_Rigidbody->getTotalForce();
			
			return { f.x(), f.y(), f.z() };
		}
		
		/**
		 * @brief Set the mass of the Rigidbody.
		 *
		 * This function sets the mass of the Rigidbody to the specified value.
		 * Values smaller than 0.005 will be clamped.
		 *
		 * @param[in] _value The new mass value for the Rigidbody.
		 */
		void Mass(const btScalar& _value) {
			m_Parameters.m_Mass = std::max(_value, static_cast<btScalar>(0.005)); // Clamp smallest mass value to 0.005.
		
			BulletReinitialise();
		}
		
		/**
		 * @brief Get the mass of the Rigidbody.
		 * @return The mass of the Rigidbody.
		 */
		[[nodiscard]] constexpr const btScalar& Mass() const noexcept {
			return m_Parameters.m_Mass;
		}
		
		/**
		 * @brief Set the drag of the Rigidbody.
		 * @param[in] _value The new drag value.
		 */
		void Drag(const btScalar& _value) {
			m_Parameters.m_Drag = _value;
			
			BulletReinitialise();
		}
		
		/**
		 * @brief Get the drag of the Rigidbody.
		 * @return The value of the drag as a const reference to a btScalar.
		 */
		[[nodiscard]] constexpr const btScalar& Drag() const noexcept {
			return m_Parameters.m_Drag;
		}
		
		/**
		 * @brief Set the angular drag of the Rigidbody.
		 * @param[in] _value The value to set the angular drag to.
		 */
		void AngularDrag(const btScalar& _value) {
			m_Parameters.m_AngularDrag = _value;
			
			BulletReinitialise();
		}
		
		/**
		 * @brief Get the angular drag of the Rigidbody.
		 * @return const btScalar& The angular drag value.
		 */
		[[nodiscard]] constexpr const btScalar& AngularDrag() const noexcept {
			return m_Parameters.m_AngularDrag;
		}
		
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
		void Friction(const btScalar& _value) {
			m_Parameters.m_Friction = _value;
			
			BulletReinitialise();
		}
		
		/**
		 * @brief Get the friction of the Rigidbody.
		 * @return The friction of the Rigidbody.
		 */
		[[nodiscard]] constexpr const btScalar& Friction() const noexcept {
			return m_Parameters.m_Friction;
		}
		
		/**
		 * @brief Set the bounciness of the Rigidbody.
		 *
		 * This function sets the bounciness value of the Rigidbody.
		 * The bounciness determines how much force is retained when the Rigidbody collides with other objects.
		 *
		 * @param[in] _value The bounciness value to be set. It should be a positive value.
		 */
		void Bounciness(const btScalar& _value) {
			m_Parameters.m_Bounciness = _value;
			
			BulletReinitialise();
		}
		
		/**
		 * @brief Get the bounciness of the Rigidbody.
		 * @return const btScalar& Returns a reference to the bounciness value.
		 */
		[[nodiscard]] constexpr const btScalar& Bounciness() const noexcept {
			return m_Parameters.m_Bounciness;
		}
		
	};
	
} // LouiEriksson::Engine::Physics

#endif //FINALYEARPROJECT_RIGIDBODY_H