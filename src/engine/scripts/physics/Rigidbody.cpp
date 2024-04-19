#include "Rigidbody.h"

#include "../core/Debug.h"
#include "../core/Time.h"
#include "../core/Transform.h"
#include "../ecs/Component.h"
#include "../ecs/GameObject.h"
#include "../physics/Collision.h"

#include "Collider.h"
#include "Physics.h"

#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <LinearMath/btDefaultMotionState.h>

#include <algorithm>
#include <cmath>
#include <exception>
#include <memory>
#include <stdexcept>
#include <vector>

namespace LouiEriksson::Engine::Physics {
	
	Rigidbody::BulletRigidbody::BulletRigidbody(const std::weak_ptr<Transform>& _transform, const std::weak_ptr<Collider>& _collider, const Parameters& _parameters) :
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
					float sweep_sphere_radius = 0.01;
					{
						const auto* const col = m_Rigidbody->getCollisionShape();
					
						const btTransform t;
						btVector3 min;
						btVector3 max;
						
						col->getAabb(t, min, max);
						
						const auto delta = max - min;
						
						// Get largest axis:
						const auto multiplier = 0.707f;
					
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
					const float threshold_Ms = sweep_sphere_radius;
					
					m_Rigidbody->setCcdMotionThreshold(Time::FixedDeltaTime() * threshold_Ms);
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
	
	Rigidbody::BulletRigidbody::~BulletRigidbody() {
		
		if (m_Rigidbody != nullptr) {
			Physics::s_DynamicsWorld->removeRigidBody(m_Rigidbody.get());
		}
	}
	
	Rigidbody::Parameters::Parameters() noexcept :
			m_Kinematic (false),
			m_UseGravity(true),
			m_Continuous(true),
			
			// See also: https://www.engineeringtoolbox.com/drag-coefficient-d_627.html
			m_Mass       (1.0),
			m_Drag       (0.2),
			m_AngularDrag(0.005f),
			m_Friction   (0.5),
			m_Bounciness (0.5),
			
			m_Inertia(1.0, 1.0, 1.0) {}
	
	Rigidbody::Rigidbody(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : ECS::Component(_parent) {}
	
	void Rigidbody::Interpolate() {
		
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
				if (glm::length(av) > 0.005f) {
					rot = glm::angleAxis(glm::cos(glm::length(av)) * Time::FixedDeltaTime(), glm::normalize(av));
				}
				else {
					rot = glm::identity<glm::quat>();
				}
				
				// Apply new values. Linearly interpolate using length of time since last physics update.
				transform->Position(lastPos + (Velocity() * Physics::s_LastTick * Time::FixedDeltaTime()));
				transform->Rotation(glm::slerp(lastRot, lastRot * rot, std::max(Physics::s_LastTick, 0.0)));
			}
		}
	}
	
	void Rigidbody::BulletReinitialise() {
		
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
	
	void Rigidbody::Sync() {
		
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
	
	const std::vector<Collision>& Rigidbody::Collisions() const noexcept {
		return m_Collisions;
	}
	
	void Rigidbody::SetTransform(const std::weak_ptr<Transform>& _transform) {
		m_Transform = _transform;
		
		BulletReinitialise();
	}
	const std::weak_ptr<Transform>& Rigidbody::GetTransform() const noexcept {
		return m_Transform;
	}
	
	void Rigidbody::SetCollider(const std::weak_ptr<Collider>& _collider) {
		
		m_Collider = _collider;
		
		if (const auto col = m_Collider.lock()) {
			col->m_CollisionShape->calculateLocalInertia(m_Parameters.m_Mass, m_Parameters.m_Inertia);
		}
		
		BulletReinitialise();
	}
	const std::weak_ptr<Collider>& Rigidbody::GetCollider() const noexcept {
		return m_Collider;
	}
	
	void Rigidbody::Position(const glm::vec3& _value) {
		
		m_Transform.lock()->Position(_value);
		
		BulletReinitialise();
	}
	const glm::vec3& Rigidbody::Position() {
		
		Sync();
		
		return m_Transform.lock()->Position();
	}
	
	void Rigidbody::Rotation(const glm::quat& _value) {
		
		m_Transform.lock()->Rotation(_value);
		
		BulletReinitialise();
	}
	const glm::quat& Rigidbody::Rotation() {
		
		Sync();
		
		return m_Transform.lock()->Rotation();
	}
	
	void Rigidbody::Kinematic(const bool& _value) {
		m_Parameters.m_Kinematic = _value;
		
		BulletReinitialise();
	}
	const bool& Rigidbody::Kinematic() const noexcept {
		return m_Parameters.m_Kinematic;
	}
	
	void Rigidbody::Gravity(const bool& _value) {
		m_Parameters.m_UseGravity = _value;
		
		BulletReinitialise();
	}
	const bool& Rigidbody::Gravity() const noexcept {
		return m_Parameters.m_UseGravity;
	}
	
	void Rigidbody::Velocity(const glm::vec3& _value) {
		
		m_Parameters.m_BulletRigidbody->m_Rigidbody->setLinearVelocity(
			{ _value.x, _value.y, _value.z }
		);
	}
	glm::vec3 Rigidbody::Velocity() const {
		
		const auto v = m_Parameters.m_BulletRigidbody->m_Rigidbody->getLinearVelocity();
		
		return {v.x(), v.y(), v.z()};
	}
	
	void Rigidbody::AngularVelocity(const glm::vec3& _value) {
		
		m_Parameters.m_BulletRigidbody->m_Rigidbody->setAngularVelocity(
			{ _value.x, _value.y, _value.z }
		);
	}
	glm::vec3 Rigidbody::AngularVelocity() const {
		
		const auto av = m_Parameters.m_BulletRigidbody->m_Rigidbody->getAngularVelocity();
		
		return { av.x(), av.y(), av.z() };
	}
	
	void Rigidbody::AddForce(const glm::vec3& _value, const glm::vec3& _relativePosition) {
	
		m_Parameters.m_BulletRigidbody->m_Rigidbody->applyForce(
			{            _value.x,            _value.y,            _value.z },
			{ _relativePosition.x, _relativePosition.y, _relativePosition.z }
		);
	}
	glm::vec3 Rigidbody::GetForce() const {
		
		const auto f = m_Parameters.m_BulletRigidbody->m_Rigidbody->getTotalForce();
		
		return { f.x(), f.y(), f.z() };
	}
	
	void Rigidbody::Mass(const float& _value) {
		m_Parameters.m_Mass = std::max(_value, 0.005f); // Clamp smallest mass value to 0.005.
	
		BulletReinitialise();
	}
	const float& Rigidbody::Mass() const noexcept {
		return m_Parameters.m_Mass;
	}
	
	void Rigidbody::Drag(const float& _value) {
		m_Parameters.m_Drag = _value;
		
		BulletReinitialise();
	}
	const float& Rigidbody::Drag() const noexcept {
		return m_Parameters.m_Drag;
	}
	
	void Rigidbody::AngularDrag(const float& _value) {
		m_Parameters.m_AngularDrag = _value;
		
		BulletReinitialise();
	}
	const float& Rigidbody::AngularDrag() const noexcept {
		return m_Parameters.m_AngularDrag;
	}
	
	void Rigidbody::Friction(const float& _value) {
		m_Parameters.m_Friction = _value;
		
		BulletReinitialise();
	}
	const float& Rigidbody::Friction() const noexcept {
		return m_Parameters.m_Friction;
	}
	
	void Rigidbody::Bounciness(const float& _value) {
		m_Parameters.m_Bounciness = _value;
		
		BulletReinitialise();
	}
	const float& Rigidbody::Bounciness() const noexcept {
		return m_Parameters.m_Bounciness;
	}
	
} // LouiEriksson::Engine::Physics