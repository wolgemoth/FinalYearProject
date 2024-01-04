#ifndef FINALYEARPROJECT_COLLIDER_H
#define FINALYEARPROJECT_COLLIDER_H

#include "Collision.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Time.h"

#include "Serialisation.h"

// @Assessor: This class was originally submitted for PFG but has been modified to accommodate Bullet Physics Engine for GEP.

namespace LouiEriksson {
	
	class Collider : public Component {
	
		friend Rigidbody;
		
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
	
		/// <summary> Set the Transform of the Collider. </summary>
		virtual void SetTransform(const std::weak_ptr<Transform>& _transform);
	
		/// <summary> Get the Transform of the Collider. </summary>
		const std::weak_ptr<Transform>& GetTransform();
	
		/// <summary> Set the Rigidbody of the Collider. </summary>
		void SetRigidbody(const std::weak_ptr<Rigidbody>& _transform);
	
		/// <summary> Get the Rigidbody of the Collider. </summary>
		const std::weak_ptr<Rigidbody>& GetRigidbody();
	
		/// <summary> Set the Collider's Type. </summary>
		void SetType(const Type& _type);
		
		/// <summary> Get the Collider's Type. </summary>
		Type GetType();
	
	protected:
		
		/* TYPE */
		Type m_Type;
	
		/* REFERENCES */
	
		/// <summary> (Bullet Physics Engine) Shape of collider. </summary>
		std::shared_ptr<btCollisionShape> m_CollisionShape;
		
		/// <summary> Transform of the Collider. </summary>
		std::weak_ptr<Transform> m_Transform;
	
		/// <summary> Rigidbody of the Collider. </summary>
		std::weak_ptr<Rigidbody> m_Rigidbody;
	};
}

#endif //FINALYEARPROJECT_COLLIDER_H