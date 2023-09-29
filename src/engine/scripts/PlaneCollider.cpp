#include "stdafx.h"

#include "PlaneCollider.h"

PlaneCollider::PlaneCollider(std::weak_ptr<GameObject> _parent) : Collider(_parent) {
}

float PlaneCollider::DistanceToPlane(glm::vec3 _point, glm::vec3 _pointOnPlane) {
	return glm::dot(_point - _pointOnPlane, Normal());
}

bool PlaneCollider::Evaluate(std::shared_ptr<Collider> _other) {

	bool result = false;

	auto vel = _other->GetRigidbody().lock()->Velocity();

	auto c0 = _other->GetTransform().lock()->m_Position;
	auto c1 = _other->GetTransform().lock()->m_Position + vel * Time::DeltaTime();

	for (float t = 0; t < 1.0f; t += Time::FixedDeltaTime()) {

		glm::vec3 ci(0.0f);

		auto q = GetTransform().lock()->m_Position;

		float r = _other->Radius();

		float d0 = DistanceToPlane(c0, q);
		float d1 = DistanceToPlane(c1, q);

		if (glm::abs(d0) <= r) {

			ci = c0;
			t = 0.0f;

			result = true;
		}
		else if (d0 > r && d1 < r) {

			t = (d0 - r) / (d0 - d1);

			ci = (1.0f - t) * c0 + (t * c1);

			result = true;
		}

		if (result == true) {
			break;
		}
	}

	return result;
}

glm::vec3 PlaneCollider::Normal() {

	const glm::vec3 axis(0.0f, 1.0f, 0.0f);

	return GetTransform().lock()->m_Rotation * axis;
}