#ifndef FINALYEARPROJECT_COLLIDER_H
#define FINALYEARPROJECT_COLLIDER_H

#include "../ecs/Component.h"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>

#include <memory>

namespace LouiEriksson::Engine {
	
	class Transform;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Physics {
	
	class Rigidbody;
	
	class Collider : public ECS::Component {
	
		friend Rigidbody;
		
	public:
	
		/// <summary> The type of the collider. </summary>
		enum Type : char {
			
			/// <summary> Collider is a Sphere Collider. </summary>
			Sphere,
			
			/// <summary> Collider is a Plane Collider. </summary>
			Plane
		};
	
		explicit Collider(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		~Collider() override = default;
	
		/// <summary> Set the Transform of the Collider. </summary>
		virtual void SetTransform(const std::weak_ptr<Transform>& _transform) noexcept;
	
		/// <summary> Get the Transform of the Collider. </summary>
		[[nodiscard]] const std::weak_ptr<Transform>& GetTransform() const noexcept;
	
		/// <summary> Set the Rigidbody of the Collider. </summary>
		void SetRigidbody(const std::weak_ptr<Rigidbody>& _transform) noexcept;
	
		/// <summary> Get the Rigidbody of the Collider. </summary>
		[[nodiscard]] const std::weak_ptr<Rigidbody>& GetRigidbody() const noexcept;
	
		/// <summary> Set the Collider's TypeID. </summary>
		void SetType(const Type& _type) noexcept;
		
		/// <summary> Get the Collider's TypeID. </summary>
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
	
} // LouiEriksson::Engine::Physics

#endif //FINALYEARPROJECT_COLLIDER_H