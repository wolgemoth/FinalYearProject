#include "stdafx.h"

#include "Rigidbody.h"

#include "Physics.h"
#include "Collider.h"

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	Rigidbody::BulletRigidbody::BulletRigidbody(const std::weak_ptr<Transform>& _transform, const std::weak_ptr<Collider>& _collider, const Parameters& _parameters) {
		
		try {
			
			// Initialise the motion state.
			m_MotionState.reset(new btDefaultMotionState());
			
			// Initialise the rigidbody.
			m_Rigidbody.reset(
				new btRigidBody(
					_parameters.m_Mass,
					m_MotionState.get(),
					_collider.lock()->m_CollisionShape.get(),
					_parameters.m_Inertia
				)
			);
			
			// Assign the drag and angular drag coefficients.
			m_Rigidbody->setDamping(_parameters.m_Drag, _parameters.m_AngularDrag);
			
			// Assign the friction coefficient.
			m_Rigidbody->setFriction(_parameters.m_Friction);
			
			// Assign the restitution (bounciness) of the rigidbody.
			m_Rigidbody->setRestitution(_parameters.m_Bounciness);
			
			// Set the transform of the rigidbody.
			{
				const auto pos  = _transform.lock()->m_Position;
				const auto quat = _transform.lock()->m_Rotation;
				
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
					m_Rigidbody->getCollisionFlags() |
					btCollisionObject::CF_KINEMATIC_OBJECT
				);
				
				// Restrict all motion.
				m_Rigidbody-> setLinearFactor({ 0.0f, 0.0f, 0.0f });
				m_Rigidbody->setAngularFactor({ 0.0f, 0.0f, 0.0f });
			}
			else {
				
				// Ensure motion is permitted along all axes.
				m_Rigidbody-> setLinearFactor({ 1.0f, 1.0f, 1.0f });
				m_Rigidbody->setAngularFactor({ 1.0f, 1.0f, 1.0f });
			}
			
			// Set up how the rigidbody interacts with gravity:
			{
				const auto g = Physics::Gravity();
				
				m_Rigidbody->setGravity(
					_parameters.m_UseGravity ?
						btVector3( g.x,  g.y,  g.z) :
						btVector3(0.0f, 0.0f, 0.0f)
				);
			}
			
			// See: https://docs.panda3d.org/1.10/python/programming/physics/bullet/ccd
			if (_parameters.m_Continuous) {
				
				// Compute the continuous sphere radius using the collider's AABB.
				float sweep_sphere_radius = 0.01f;
				{
					const auto* col = m_Rigidbody->getCollisionShape();
				
					btTransform t;
					btVector3 min;
					btVector3 max;
					
					col->getAabb(t, min, max);
					
					const auto delta = max - min;
					
					// Get largest axis:
					const auto multiplier = 0.707f;
				
					sweep_sphere_radius = glm::max(
						sweep_sphere_radius,
						glm::max(
							delta.x(),
							glm::max(
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
			
			// Add the rigidbody to the dynamics world.
			Physics::s_DynamicsWorld->addRigidBody(m_Rigidbody.get());
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
		}
	}
	
	Rigidbody::Parameters::Parameters() {
		
		m_BulletRigidbody = std::shared_ptr<BulletRigidbody>(nullptr);
		
		m_Kinematic  = false;
		m_UseGravity = true;
		m_Continuous = true;
		
		// See also: https://www.engineeringtoolbox.com/drag-coefficient-d_627.html
		
		m_Mass        = 1.0f;
		m_Drag        = 0.2f;
		m_AngularDrag = 0.005f;
		m_Friction    = 0.5f;
		m_Bounciness  = 0.5f;
		
		m_Inertia = { 1.0f, 1.0f, 1.0f };
	}
	
	Rigidbody::BulletRigidbody::~BulletRigidbody() {
		
		if (m_Rigidbody != nullptr) {
			Physics::s_DynamicsWorld->removeRigidBody(m_Rigidbody.get());
		}
	}
	
	Rigidbody::Rigidbody(const std::shared_ptr<GameObject>& _parent) : Component(_parent) {
		
		m_Transform = std::shared_ptr<Transform>(nullptr);
		m_Collider  = std::shared_ptr<Collider> (nullptr);
	}
	
	void Rigidbody::Interpolate() {
		
		if (Physics::s_LastTick > 0.0f) {
			
			const auto transform = m_Transform.lock();
			
			if (transform != nullptr) {
				
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
				transform->m_Position = lastPos + (Velocity() * Physics::s_LastTick * Time::FixedDeltaTime());
				transform->m_Rotation = glm::slerp(lastRot, lastRot * rot, glm::max(Physics::s_LastTick, 0.0f));
			}
		}
	}
	
	void Rigidbody::BulletReinitialise() {
		
		try {
			const auto transform = m_Transform.lock();
			const auto collider  =  m_Collider.lock();
			
			if (transform != nullptr &&
			    collider != nullptr) {
				
				m_Parameters.m_BulletRigidbody.reset(
					new BulletRigidbody(
						m_Transform,
						m_Collider,
						m_Parameters
					)
				);
			}
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
		}
	}
	
	void Rigidbody::Sync() {
		
		try {
			
			const auto transform = m_Transform.lock();
			
			if (transform != nullptr) {
				
				// Get the transform from the bullet physics engine.
				auto t = m_Parameters.m_BulletRigidbody->m_Rigidbody->getWorldTransform();
				
				const auto bOrigin   = t.getOrigin();
				const auto bRotation = t.getRotation().inverse();
				
				// Sync the transform from bullet with the transform in-engine.
				transform->m_Position = glm::vec3(  bOrigin.x(),   bOrigin.y(),   bOrigin.z());
				transform->m_Rotation = glm::quat(bRotation.w(), bRotation.x(), bRotation.y(), bRotation.z());
			}
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
		}
	}
	
	void Rigidbody::SetTransform(const std::weak_ptr<Transform>& _transform) {
		m_Transform = _transform;
		
		BulletReinitialise();
	}
	const std::weak_ptr<Transform>& Rigidbody::GetTransform() const {
		return m_Transform;
	}
	
	void Rigidbody::SetCollider(const std::weak_ptr<Collider>& _collider) {
		
		const auto col = _collider.lock();
		
		m_Collider = col;
		
		if (col != nullptr) {
			col->m_CollisionShape->calculateLocalInertia(m_Parameters.m_Mass, m_Parameters.m_Inertia);
		}
		
		BulletReinitialise();
	}
	const std::weak_ptr<Collider>& Rigidbody::GetCollider() const {
		return m_Collider;
	}
	
	void Rigidbody::Position(const glm::vec3& _value) {
		
		m_Transform.lock()->m_Position = _value;
		
		BulletReinitialise();
	}
	const glm::vec3& Rigidbody::Position() {
		
		Sync();
		
		return m_Transform.lock()->m_Position;
	}
	
	void Rigidbody::Rotation(const glm::quat& _value) {
		
		m_Transform.lock()->m_Rotation = _value;
		
		BulletReinitialise();
	}
	const glm::quat& Rigidbody::Rotation() {
		
		Sync();
		
		return m_Transform.lock()->m_Rotation;
	}
	
	void Rigidbody::Kinematic(const bool& _value) {
		m_Parameters.m_Kinematic = _value;
		
		BulletReinitialise();
	}
	const bool& Rigidbody::Kinematic() const {
		return m_Parameters.m_Kinematic;
	}
	
	void Rigidbody::Gravity(const bool& _value) {
		m_Parameters.m_UseGravity = _value;
		
		BulletReinitialise();
	}
	const bool& Rigidbody::Gravity() const {
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
	const float& Rigidbody::Mass() const {
		return m_Parameters.m_Mass;
	}
	
	void Rigidbody::Drag(const float& _value) {
		m_Parameters.m_Drag = _value;
		
		BulletReinitialise();
	}
	const float& Rigidbody::Drag() const {
		return m_Parameters.m_Drag;
	}
	
	void Rigidbody::AngularDrag(const float& _value) {
		m_Parameters.m_AngularDrag = _value;
		
		BulletReinitialise();
	}
	const float& Rigidbody::AngularDrag() const {
		return m_Parameters.m_AngularDrag;
	}
	
	void Rigidbody::Friction(const float& _value) {
		m_Parameters.m_Friction = _value;
		
		BulletReinitialise();
	}
	const float& Rigidbody::Friction() const {
		return m_Parameters.m_Friction;
	}
	
	void Rigidbody::Bounciness(const float& _value) {
		m_Parameters.m_Bounciness = _value;
		
		BulletReinitialise();
	}
	const float& Rigidbody::Bounciness() const {
		return m_Parameters.m_Bounciness;
	}
}