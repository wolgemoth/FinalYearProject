#include "stdafx.h"

#include "Rigidbody.h"

#include "Physics.h"
#include "Collider.h"

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	Rigidbody::BulletRigidbody::BulletRigidbody(const std::weak_ptr<Transform>& _transform, const std::weak_ptr<Collider>& _collider, const Parameters& _parameters) {
		
		try {
			
			m_MotionState.reset(new btDefaultMotionState());
			
			m_Rigidbody.reset(
				new btRigidBody(
					_parameters.m_Mass,
					m_MotionState.get(),
					_collider.lock()->m_CollisionShape.get()
				)
			);
			
			// Assign the drag and angular drag coefficients.
			m_Rigidbody->setDamping(_parameters.m_Drag, _parameters.m_AngularDrag);
			
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
				
				// Reset the velocities:
				m_Rigidbody->setLinearFactor(btVector3(0, 0, 0));
				m_Rigidbody->setAngularFactor(btVector3(0, 0, 0));
			}
			
			// Set up how the rigidbody interacts with gravity:
			{
				const auto g = Physics::Gravity();
				
				m_Rigidbody->setGravity(
					_parameters.m_UseGravity ?
						btVector3(g.x, g.y, g.z) :
						btVector3(0, 0, 0)
				);
			}
			
			// Add the rigidbody to the dynamics world.
			Physics::s_DynamicsWorld->addRigidBody(m_Rigidbody.get());
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
		}
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
	
	Rigidbody::Parameters::Parameters() {
		
		m_BulletRigidbody = std::shared_ptr<BulletRigidbody>(nullptr);
		
		m_Kinematic  = false;
		m_UseGravity = true;
		
		m_Velocity        = glm::vec3(0.0f);
		m_AngularVelocity = glm::vec3(0.0f);
		m_Force           = glm::vec3(0.0f);
		
		m_Mass        = 1.0f;
		m_Drag        = 0.005f; // Courtesy of: https://www.engineeringtoolbox.com/drag-coefficient-d_627.html
		m_AngularDrag = 0.0f;
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
				btTransform t;
				m_Parameters.m_BulletRigidbody->m_MotionState->getWorldTransform(t);
				
				const auto bOrigin   = t.getOrigin();
				const auto bRotation = t.getRotation();
				
				//printf("%.1f, %.1f, %.1f, %.1f\n", bOrigin.x(), bOrigin.y(), bOrigin.z(), bOrigin.w());
				
				// Sync the transform from bullet with the transform in-engine.
				transform->m_Position = glm::vec3(  bOrigin.x(),   bOrigin.y(),   bOrigin.z());
				transform->m_Rotation = glm::quat(bRotation.w(), bRotation.x(), bRotation.y(), bRotation.z());
			}
		}
		catch (const std::exception e) {
			std::cout << e.what() << '\n';
		}
	}
	
	void Rigidbody::SetTransform(const std::weak_ptr<Transform>& _transform) {
		m_Transform = _transform;
		
		BulletReinitialise();
	}
	std::weak_ptr<Transform> Rigidbody::GetTransform() {
		return m_Transform;
	}
	
	void Rigidbody::SetCollider(const std::weak_ptr<Collider>& _transform) {
		m_Collider = _transform;
		
		BulletReinitialise();
	}
	std::weak_ptr<Collider> Rigidbody::GetCollider() {
		return m_Collider;
	}
	
	void Rigidbody::Kinematic(const bool& _value) {
		m_Parameters.m_Kinematic = _value;
		
		BulletReinitialise();
	}
	
	const bool& Rigidbody::Kinematic() {
		return m_Parameters.m_Kinematic;
	}
	
	void Rigidbody::Gravity(const bool& _value) {
		m_Parameters.m_UseGravity = _value;
		
		BulletReinitialise();
	}
	
	const bool& Rigidbody::Gravity() {
		return m_Parameters.m_UseGravity;
	}
	
	void Rigidbody::Velocity(const glm::vec3& _velocity) {
		m_Parameters.m_Velocity = _velocity;
		
		BulletReinitialise();
	}
	glm::vec3 Rigidbody::Velocity() {
		return m_Parameters.m_Velocity;
	}
	
	void Rigidbody::AngularVelocity(const glm::vec3& _angularVelocity) {
		m_Parameters.m_AngularVelocity = _angularVelocity;
		
		BulletReinitialise();
	}
	glm::vec3 Rigidbody::AngularVelocity() {
		return m_Parameters.m_AngularVelocity;
	}
	
	void Rigidbody::AddForce(const glm::vec3& _force) {
		m_Parameters.m_Force += _force;
		
		BulletReinitialise();
	}
	void Rigidbody::ClearForce() {
		m_Parameters.m_Force = glm::vec3(0.0f);
		
		BulletReinitialise();
	}
	
	glm::vec3 Rigidbody::GetForce() {
		return m_Parameters.m_Force;
	}
	
	void Rigidbody::Mass(const float& _mass) {
		m_Parameters.m_Mass = std::max(_mass, 0.005f); // Clamp smallest mass value to 0.005.
	
		BulletReinitialise();
	}
	float Rigidbody::Mass() const {
		return m_Parameters.m_Mass;
	}
	
	void Rigidbody::Drag(const float& _drag) {
		m_Parameters.m_Drag = _drag;
		
		BulletReinitialise();
	}
	float Rigidbody::Drag() const {
		return m_Parameters.m_Drag;
	}
	
	void Rigidbody::AngularDrag(const float& _angularDrag) {
		m_Parameters.m_AngularDrag = _angularDrag;
		
		BulletReinitialise();
	}
	float Rigidbody::AngularDrag() const {
		return m_Parameters.m_AngularDrag;
	}
}