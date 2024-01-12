#include "Collider.h"

#include "../core/Transform.h"
#include "../ecs/Component.h"
#include "../ecs/GameObject.h"
#include "../physics/Rigidbody.h"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>

#include <memory>

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	Collider::Collider(const std::shared_ptr<GameObject>& _parent) : Component(_parent) {
		
		m_CollisionShape = std::shared_ptr<btCollisionShape>(nullptr);
		
		m_Transform = std::shared_ptr<Transform>(nullptr);
		m_Rigidbody = std::shared_ptr<Rigidbody>(nullptr);
		
		m_Type = Type::Sphere;
	}
	
	void Collider::SetTransform(const std::weak_ptr<Transform>& _transform) {
		m_Transform = _transform;
	}
	const std::weak_ptr<Transform>& Collider::GetTransform() {
		return m_Transform;
	}
	
	void Collider::SetRigidbody(const std::weak_ptr<Rigidbody>& _transform) {
		m_Rigidbody = _transform;
	}
	const std::weak_ptr<Rigidbody>& Collider::GetRigidbody() {
		return m_Rigidbody;
	}
	
	void Collider::SetType(const Type& _type) {
		m_Type = _type;
	}
	Collider::Type Collider::GetType() {
		return m_Type;
	}
}