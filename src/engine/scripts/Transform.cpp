#include "stdafx.h"

#include "Transform.h"

namespace LouiEriksson {
	
	Transform::Transform(std::weak_ptr<GameObject> _parent) : Component(std::move(_parent)) {
		m_Position = glm::vec3(0.0f);
		m_Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		m_Scale    = glm::vec3(1.0f);
	}
	
	glm::mat4 Transform::TRS() const {
		return glm::scale(glm::translate(glm::mat4(1.0f), m_Position) * glm::mat4_cast(glm::inverse(m_Rotation)), m_Scale);
	}
}