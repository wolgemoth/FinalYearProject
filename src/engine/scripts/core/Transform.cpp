#include "Transform.h"

#include "../ecs/Component.h"
#include "../ecs/GameObject.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <memory>

namespace LouiEriksson::Engine {
	
	Transform::Transform(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : Component(_parent),
			m_Position(VEC_ZERO     ),
			m_Rotation(QUAT_IDENTITY),
			m_Scale   (VEC_ONE      ) { }

	/// <summary> Get the given vector (local to this transform) as it exists in world space.</summary>
	glm::vec3 Transform::ToWorld(const glm::vec3& _vector) const {
		return _vector * m_Rotation;
	}
	
	glm::mat4 Transform::TRS() const {
		return
			glm::scale(
				glm::translate(glm::mat4(1.0f), m_Position) *
				glm::mat4_cast(glm::inverse(m_Rotation)
			),
			m_Scale
		);
	}
	
} // LouiEriksson::Engine