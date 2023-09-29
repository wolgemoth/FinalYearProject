#include "stdafx.h"

#include "Collider.h"

Collider::Collider(std::weak_ptr<GameObject> _parent) : Component(_parent) {
	m_Transform = std::shared_ptr<Transform>(nullptr); 
	m_Rigidbody = std::shared_ptr<Rigidbody>(nullptr);

	m_Radius = 0;

	m_Type = Type::Sphere;
}

bool Collider::Evaluate(std::shared_ptr<Collider> _other, Collision* _hit) {

	bool result = false;

	// Ignore self-collision.
	if (_other.get() != this) {

		auto t1 =   this->GetType();
		auto t2 = _other->GetType();

		// Check for the type of collision and perform it.
		if (t1 == Collider::Sphere && t2 == Collider::Sphere) {

			result = SphereOnSphere(_other, _hit);
		}
		else if ((t1 == Collider::Plane  && t2 == Collider::Sphere) ||
			     (t1 == Collider::Sphere && t2 == Collider::Plane)) {

			result = SphereOnPlane(_other, _hit);
		}
		else {
			throw std::exception("Undefinined collision. Please implement a response.");
		}
	}

	return result;
}

void Collider::SetTransform(std::weak_ptr<Transform> _transform) {
	m_Transform = _transform;
}
std::weak_ptr<Transform> Collider::GetTransform() {
	return m_Transform;
}

void Collider::SetRigidbody(std::weak_ptr<Rigidbody> _transform) {
	m_Rigidbody = _transform;
}
std::weak_ptr<Rigidbody> Collider::GetRigidbody() {
	return m_Rigidbody;
}

void Collider::SetType(const Type& _type) {
	m_Type = _type;
}
Collider::Type Collider::GetType() {
	return m_Type;
}

void Collider::Radius(const float& _other) {
	m_Radius = _other;
}
float Collider::Radius() {
	return m_Radius;
}

bool Collider::SphereOnSphere(std::shared_ptr<Collider> _other, Collision* _hit) {

	bool result = false;

	// Get dir between this and other collider.
	auto delta = GetTransform().lock()->m_Position -
		 _other->GetTransform().lock()->m_Position;

	// Squared magnitude of length for comparisons (faster because it skips sqrt).
	float sqrMag = glm::length2(delta);

	// Threshold under which the spheres are considered to be colliding.
	float threshold = (Radius() + _other->Radius());

	// Perform square distance comparison.
	result = sqrMag < (threshold * threshold);

	if (result) {

		glm::vec3 contactPoint(0.0f);

		// Normalise the direction.
		float mag = glm::sqrt(sqrMag);
		auto normalisedDir = (delta / mag);

		// Compute contact point from intersection of spheres.
		contactPoint = (normalisedDir * ((Radius() * 2.0f) - mag));

		// Calculate the normal of the collision.
		auto normal = glm::normalize(
			        GetTransform().lock()->m_Position -
			_other->GetTransform().lock()->m_Position
		);

		_hit->ContactPoint(contactPoint);
		_hit->Impulse(SphereImpulse(_other->GetRigidbody().lock(), normal));
		_hit->Normal(-normal);

		//std::cout << "\t\t" << Serialisation::Serialise(_hit->ContactPoint()) << " Mag(" << glm::length(_hit->ContactPoint()) << ")" << "\n";
	}

	return result;
}

bool Collider::SphereOnPlane(std::shared_ptr<Collider> _other, Collision* _hit) {

	bool result = false;

	// Get the velocity of the sphere.
	auto velocity = GetRigidbody().lock()->Velocity();

	// Get start and compute end sphere position.
	auto pos_start = GetTransform().lock()->m_Position;
	auto pos_end   = GetTransform().lock()->m_Position + velocity * Time::DeltaTime();

	glm::vec3 pos_curr(0.0f);

	// Get any point on the plane. In this case the center.
	auto point_on_plane = _other->GetTransform().lock()->m_Position;

	float t; // Used later for lerp.

	// Compute distance from start and end points to plane.
	float distance_start = DistanceToPlane(pos_start, point_on_plane);
	float distance_end   = DistanceToPlane(pos_end, point_on_plane);

	// Evaluate if sphere is already intersecting.
	if (glm::abs(distance_start) <= Radius()) {

		// Only gets called if already intersecting.
		pos_curr = pos_start;
		t = 0.0f;

		result = true;
	}
	// Otherwise check if either start or end is overlapping.
	else if (distance_start > Radius() && distance_end < Radius()) {
		
		t = (distance_start - Radius()) / (distance_start - distance_end); // Lerp.
		pos_curr = (1.0f - t) * pos_start + (t * pos_end);

		result = true;
	}

	if (result) {

		// Compute amount the sphere is "inside" the plane.
		auto overshoot = pos_curr - (PlanarNormal() * Radius());

		// Get the distance from the current point to the plane.
		float distance_curr = DistanceToPlane(pos_curr, point_on_plane);

		// Assign hit information.
		_hit->ContactPoint(PlanarNormal() * (Radius() - distance_curr));
		_hit->Impulse(PlanarImpulse(GetRigidbody().lock(), -PlanarNormal())); // Planar normal is negative because that is the collision normal.
		_hit->Normal(PlanarNormal());
	}

	return result;
}

glm::vec3 Collider::PlanarImpulse(std::shared_ptr<Rigidbody> _other, glm::vec3 _normal) {

	// See Collider::SphereImpulse.
	// This is the same math but assuming that the other object is static.

	glm::vec3 result(0.0f);

	const float elasticity = 0.5f;

	auto coefficient = -(1.0f + elasticity);
	auto delta = -_other->Velocity();
	auto similarity = glm::dot(delta, _normal);

	return -_normal * ((coefficient * similarity) * _other->Mass());
}

glm::vec3 Collider::SphereImpulse(std::shared_ptr<Rigidbody> _other, glm::vec3 _normal) {

	glm::vec3 result(0.0f);

	// How "bouncy" this rigidbody is.
	const float elasticity = 0.9f;

	// Lock reference to this rb for easy access.
	auto rbThis = GetRigidbody().lock();

	// Compute 'numerator' components of the equation.
	auto coefficient = -(1.0f + elasticity);
	auto delta = rbThis->Velocity() - _other->Velocity();
	auto similarity = glm::dot(delta, _normal);

	// Compute 'denominator' components of the equation.
	float r1 = 1.0f / rbThis->Mass();
	float r2 = 1.0f / _other->Mass();

	// Output result (everything combined.)
	return _normal * ((coefficient * similarity) / (r1 + r2));
}

float Collider::DistanceToPlane(glm::vec3 _point, glm::vec3 _pointOnPlane) {

	// Compute distance to plane.
	return glm::dot(_point - _pointOnPlane, PlanarNormal());
}

glm::vec3 Collider::PlanarNormal() {

	const glm::vec3 axis(0.0f, 1.0f, 0.0f);

	// Rotate axis by rotation and return.
	return GetTransform().lock()->m_Rotation * axis;
}