#ifndef FINALYEARPROJECT_COLLIDER_H
#define FINALYEARPROJECT_COLLIDER_H

#include "../core/Transform.h"
#include "../ecs/Component.h"

#include "Rigidbody.h"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>

#include <memory>

// @Assessor: This class was originally submitted for PFG but has been modified to accommodate Bullet Physics Engine for GEP.

namespace LouiEriksson {
	
	class Collider : public Component {
	
		friend Rigidbody;
		
	public:
	
		/// <summary> The type of the collider. </summary>
		enum Type : char {
			
			/// <summary> Collider is a Sphere Collider. </summary>
			Sphere,
			
			/// <summary> Collider is a Plane Collider. </summary>
			Plane
		};
	
		explicit Collider(const std::shared_ptr<GameObject>& _parent) noexcept;
		~Collider() override = default;
	
		/// <summary> Set the Transform of the Collider. </summary>
		virtual void SetTransform(const std::weak_ptr<Transform>& _transform) noexcept;
	
		/// <summary> Get the Transform of the Collider. </summary>
		[[nodiscard]] const std::weak_ptr<Transform>& GetTransform() const noexcept;
	
		/// <summary> Set the Rigidbody of the Collider. </summary>
		void SetRigidbody(const std::weak_ptr<Rigidbody>& _transform) noexcept;
	
		/// <summary> Get the Rigidbody of the Collider. </summary>
		[[nodiscard]] const std::weak_ptr<Rigidbody>& GetRigidbody() const noexcept;
	
		/// <summary> Set the Collider's Type. </summary>
		void SetType(const Type& _type) noexcept;
		
		/// <summary> Get the Collider's Type. </summary>
		[[nodiscard]] const Type& GetType() const noexcept;
	
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
	
} // LouiEriksson

#endif //FINALYEARPROJECT_COLLIDER_H