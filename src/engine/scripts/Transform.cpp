#include "stdafx.h"

#include "Transform.h"

namespace LouiEriksson {
	
	Transform::Transform(const std::shared_ptr<GameObject>& _parent) : Component(_parent) {
		
		m_Position = VEC_ZERO;
		m_Rotation = QUAT_IDENTITY;
		m_Scale    = VEC_ONE;
	}

	/// <summary> Get the given vector (local to this transform) as it exists in world space.</summary>
	glm::vec3 Transform::ToWorld(const glm::vec3& _vector) const {
		return _vector * m_Rotation;
	}
	
	glm::mat4 Transform::TRS() const {
		return glm::scale(glm::translate(glm::mat4(1.0f), m_Position) * glm::mat4_cast(glm::inverse(m_Rotation)), m_Scale);
	}
}