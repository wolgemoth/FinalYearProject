#include "stdafx.h"

#include "PlaneCollider.h"

namespace LouiEriksson {
	
	PlaneCollider::PlaneCollider(std::weak_ptr<GameObject> _parent) : Collider(std::move(_parent)) {}
	PlaneCollider::~PlaneCollider() = default;
	
	float PlaneCollider::DistanceToPlane(glm::vec3 _point, glm::vec3 _pointOnPlane) {
		return glm::dot(_point - _pointOnPlane, Normal());
	}
	
	bool PlaneCollider::Evaluate(const std::shared_ptr<Collider>& _other) {
	
		bool result = false;
	
		auto vel = _other->GetRigidbody().lock()->Velocity();
	
		auto c0 = _other->GetTransform().lock()->m_Position;
		auto c1 = _other->GetTransform().lock()->m_Position + vel * Time::DeltaTime();
	
		float t;
		while(t < 0.0f) {
			
			auto q = GetTransform().lock()->m_Position;
	
			float r = _other->Radius();
	
			float d0 = DistanceToPlane(c0, q);
			float d1 = DistanceToPlane(c1, q);
	
			if (glm::abs(d0) <= r || (d0 > r && d1 < r)) {
				result = true;
				
				break;
			}
			else {
				t += Time::FixedDeltaTime();
			}
		}
	
		return result;
	}
	
	glm::vec3 PlaneCollider::Normal() {
	
		const glm::vec3 axis(0.0f, 1.0f, 0.0f);
	
		return GetTransform().lock()->m_Rotation * axis;
	}
}