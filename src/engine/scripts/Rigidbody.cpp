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
		
		m_Velocity        = glm::vec3(0.0f);
		m_AngularVelocity = glm::vec3(0.0f);
		m_Force           = glm::vec3(0.0f);
		
		m_Mass        = 0.0f;
		m_Drag        = 0.0f;
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
				
				// Sync the transform from bullet with the transform in-engine.
				transform->m_Position = glm::vec3(  bOrigin.x(),   bOrigin.y(),   bOrigin.z());
				transform->m_Rotation = glm::quat(bRotation.x(), bRotation.y(), bRotation.z(), bRotation.w());
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
	
	void Rigidbody::Euler(const float& _delta) {
	
		/* Implementation of Euler Integration */
	
		m_Parameters.m_Velocity += (m_Parameters.m_Force / m_Parameters.m_Mass) * _delta;
	
		m_Transform.lock()->m_Position += m_Parameters.m_Velocity * _delta;
	}
	
	glm::vec3 Rigidbody::Force(const float& _mass, glm::vec3 _velocity) {
		return _velocity * _mass; // Newton's F = ma.
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
	
	glm::vec3 Rigidbody::DragForce(const float& _radius) {
	
		const float airDensity = 1.0f; // 1 atm
		
		// Courtesy of: https://www.calculatordonkey.com/math/surface-area-sphere-calculator.html
		float referenceArea = 4.0f * (glm::pi<float>() * (_radius * _radius)) / 2.0f;
	
		// Courtesy of: https://www.grc.nasa.gov/WWW/k-12/airplane/drageq.html
		// This code snippet initializes a 3D vector called "result" with all components set to 0.0. It then calculates the magnitude of a velocity vector and stores it in the variable "len".
		float v = glm::length2(Velocity());
	
		float drag = Drag() * ((airDensity * (v)) * 0.5f) * referenceArea;
	
		// Compute the force of the drag from the current velocity and mass.
		glm::vec3 force = -Velocity() * drag * Mass();
	
		return force;
	}
	
	glm::vec3 Rigidbody::FrictionForce(Collision& _collision) {
	
		glm::vec3 result(0.0f);
	
		const float coefficient = 0.2f;
	
		// Velocity vector and its magnitude.
		auto dir = Velocity();
		auto len = glm::length(dir);
	
		if (len != 0) {
	
			// Project the velocity onto the normal.
			auto proj = dir - _collision.Normal() * glm::dot(dir, _collision.Normal());
	
			// Objects with more momentum going "into" a surface should receive more friction.
			float response = 1.0f - (glm::dot(proj, dir) / len);
	
			// Friction should never speed up an object.
			if (response > 0.0f) {
	
				// Compute friction by multiplying the values.
				// Courtesy of: https://www.toppr.com/guides/physics-formula/friction-formula/
				result = -proj * coefficient * response * Mass();
			}
		}
	
		return result;
	}
	
	void Rigidbody::AngularDrag(const float& _angularDrag) {
		m_Parameters.m_AngularDrag = _angularDrag;
		
		BulletReinitialise();
	}
	float Rigidbody::AngularDrag() const {
		return m_Parameters.m_AngularDrag;
	}
}