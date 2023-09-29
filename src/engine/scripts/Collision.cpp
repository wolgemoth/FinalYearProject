#include "stdafx.h"

#include "Collision.h"

Collision::Collision() {
	m_ContactPoint = glm::vec3(-1.0f);
	m_Normal       = glm::vec3( 0.0f);
	m_Impulse      = glm::vec3( 0.0f);
}

void Collision::ContactPoint(const glm::vec3& _contactPoint) {
	m_ContactPoint = _contactPoint;
}
const glm::vec3& Collision::ContactPoint() {
	return m_ContactPoint;
}

void Collision::Normal(const glm::vec3& _normal) {
	m_Normal = _normal;
}
const glm::vec3& Collision::Normal() {
	return m_Normal;
}

void Collision::Impulse(const glm::vec3& _impulse) {
	m_Impulse = _impulse;
}
const glm::vec3& Collision::Impulse() {
	return m_Impulse;
}