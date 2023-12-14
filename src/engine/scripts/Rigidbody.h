#ifndef FINALYEARPROJECT_RIGIDBODY_H
#define FINALYEARPROJECT_RIGIDBODY_H

#include "Transform.h"
#include "Collision.h"

namespace LouiEriksson {
	
	class Collider;
	
	/// <summary>
	/// Rigidbody Component implementing Rigidbody dynamics and Collision response behaviour.
	/// </summary>
	class Rigidbody : public Component {
	
	private:
	
		/// <summary> Transform of the Rigidbody </summary>
		std::weak_ptr<Transform> m_Transform;
		
		/// <summary> Collider of the Rigidbody. </summary>
		std::weak_ptr<Collider>  m_Collider;
	
		/// <summary> Velocity of the Rigidbody. </summary>
		glm::vec3 m_Velocity;
		
		/// <summary> Angular velocity of the Rigidbody. </summary>
		glm::vec3 m_AngularVelocity;
	
		/// <summary> Force of the Rigidbody. </summary>
		glm::vec3 m_Force;
	
		/// <summary> Mass of the Rigidbody. </summary>
		float m_Mass;
		
		/// <summary> Drag of the Rigidbody. </summary>
		float m_Drag;
	
		/// <summary> Angular drag of the Rigidbody. </summary>
		float m_AngularDrag;
	
		/// <summary> Compute the force from mass and velocity. </summary>
		static glm::vec3 Force(const float& _mass, glm::vec3 _velocity);
	
	public:
	
		explicit Rigidbody(const std::shared_ptr<GameObject>& _parent);
		~Rigidbody() override = default;
	
		/// <summary> Set the Transform of the Rigidbody. </summary>
		void SetTransform(const std::weak_ptr<Transform>& _transform);
	
		/// <summary> Get the Transform of the Rigidbody. </summary>
		std::weak_ptr<Transform> GetTransform();
	
		/// <summary> Set the Collider of the Rigidbody. </summary>
		void SetCollider(const std::weak_ptr<Collider>& _transform);
	
		/// <summary> Get the Collider of the Rigidbody. </summary>
		std::weak_ptr<Collider> GetCollider();
	
		/// <summary> Set the velocity of the Rigidbody. </summary>
		void Velocity(const glm::vec3& _velocity);
		
		/// <summary> Get the velocity of the Rigidbody. </summary>
		glm::vec3 Velocity();
		
		/// <summary> Set the angular velocity of the Rigidbody. </summary>
		void AngularVelocity(const glm::vec3& _angularVelocity);
	
		/// <summary> Get the angular velocity of the Rigidbody. </summary>
		glm::vec3 AngularVelocity();
	
		/// <summary> Add a force to the Rigidbody. </summary>
		void AddForce(const glm::vec3& _force);
	
		/// <summary>
		/// Clear the force of the Rigidbody.
		/// </summary>
		void ClearForce();
	
		/// <summary> Compute the drag force of the Rigidbody. </summary>
		glm::vec3 DragForce(const float& _radius);
	
		/// <summary> Compute the friction force of the Rigidbody given Collision information. </summary>
		glm::vec3 FrictionForce(Collision& _collision);
	
		/// <summary> Get the force of the Rigidbody. </summary>
		glm::vec3 GetForce();
	
		/// <summary> Compute the Euler integration. </summary>
		void Euler(const float& _delta);
	
		/// <summary> Set the mass of the Rigidbody. </summary>
		void Mass(const float& _mass);
		
		/// <summary> Get the mass of the Rigidbody. </summary>
		[[nodiscard]] float Mass() const;
	
		/// <summary> Set the drag of the Rigidbody. </summary>
		void Drag(const float& _drag);
	
		/// <summary> Get the drag of the Rigidbody. </summary>
		[[nodiscard]] float Drag() const;
	
		/// <summary> Set the angular drag of the Rigidbody. </summary>
		void AngularDrag(const float& _angularDrag);
	
		/// <summary> Get the angular drag of the Rigidbody. </summary>
		[[nodiscard]] float AngularDrag() const;
	};
}

#endif //FINALYEARPROJECT_RIGIDBODY_H