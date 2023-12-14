#ifndef FINALYEARPROJECT_COLLIDER_H
#define FINALYEARPROJECT_COLLIDER_H

#include "Collision.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Time.h"

#include "Serialisation.h"

namespace LouiEriksson {
	
	class Collider : public Component {
	
	public:
	
		/// <summary> The type of the collider. </summary>
		enum Type {
			/// <summary> Collider is a Sphere Collider. </summary>
			Sphere,
			/// <summary> Collider is a Plane Collider. </summary>
			Plane
		};
	
		explicit Collider(const std::shared_ptr<GameObject>& _parent);
		~Collider() override = default;
	
		/// <summary>
		/// Check if the Collider overlaps another.
		/// </summary>
		/// <param name="_other">Other Collider.</param>
		/// <param name="_hit">Information about the collision.</param>
		/// <returns>True if successful.</returns>
		bool Evaluate(const std::shared_ptr<Collider>& _other, Collision* _hit);
	
		/// <summary> Set the Transform of the Collider. </summary>
		void SetTransform(std::weak_ptr<Transform> _transform);
	
		/// <summary> Get the Transform of the Collider. </summary>
		std::weak_ptr<Transform> GetTransform();
	
		/// <summary> Set the Rigidbody of the Collider. </summary>
		void SetRigidbody(std::weak_ptr<Rigidbody> _transform);
	
		/// <summary> Get the Rigidbody of the Collider. </summary>
		std::weak_ptr<Rigidbody> GetRigidbody();
	
		/// <summary> Set the Collider's Type. </summary>
		void SetType(const Type& _type);
		
		/// <summary> Get the Collider's Type. </summary>
		Type GetType();
		
		/// <summary> Set the Sphere Collider's radius. </summary>
		virtual void Radius(const float& _other);
		
		/// <summary> Get the Sphere Collider's radius. </summary>
		virtual float Radius();
	
	private:
	
		/* REFERENCES */
	
		/// <summary> Transform of the Collider. </summary>
		std::weak_ptr<Transform> m_Transform;
	
		/// <summary> Rigidbody of the Collider. </summary>
		std::weak_ptr<Rigidbody> m_Rigidbody;
	
		/* TYPE */
		Type m_Type;
	
		/* SPHERE */
	
		/// <summary> SphereCollider's Radius. </summary>
		float m_Radius;
	
		/* PLANE */
	
		/// <summary> Check for overlap between two SphereColliders. </summary>
		bool SphereOnSphere(const std::shared_ptr<Collider>& _other, Collision* _hit);
	
		/// <summary> Check for overlap between a SphereCollider and a PlaneCollider. </summary>
		bool SphereOnPlane(const std::shared_ptr<Collider>& _other, Collision* _hit);
	
		/// <summary> Compute the impulse between a Spherical Rigidbody and a PlaneCollider. </summary>
		static glm::vec3 PlanarImpulse(const std::shared_ptr<Rigidbody>& _other, glm::vec3 _normal);
		
		/// <summary> Compute the impulse between a Spherical Rigidbody and SphereCollider. </summary>
		glm::vec3 SphereImpulse(const std::shared_ptr<Rigidbody>& _other, glm::vec3 _normal);
	
		/// <summary> Compute the distance to a plane. </summary>
		float DistanceToPlane(glm::vec3 _point, glm::vec3 _pointOnPlane);
	
		/// <summary> Normal of the PlaneCollider. </summary>
		glm::vec3 PlanarNormal();
	};
}

#endif //FINALYEARPROJECT_COLLIDER_H