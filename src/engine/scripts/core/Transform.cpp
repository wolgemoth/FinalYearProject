#include "Transform.h"

#include "../ecs/Component.h"
#include "../ecs/GameObject.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/quaternion.hpp>

#include <memory>

namespace LouiEriksson::Engine {
	
	Transform::Transform(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : Component(_parent),
			m_Transformation(Transformation::Everything),
			m_Position(VEC_ZERO     ),
			m_Rotation(QUAT_IDENTITY),
			m_Scale   (VEC_ONE      ) { }

	constexpr glm::vec3 Transform::ToWorld(const glm::vec3& _vector) const noexcept {
		return _vector * m_Rotation;
	}
	
	constexpr const glm::vec3& Transform::Position() const noexcept {
		return m_Position;
	}
	void Transform::Position(const glm::vec3& _position) noexcept {
		
		SetFlag(Translated);
		
		m_Position = _position;
	}
	
	constexpr const glm::quat& Transform::Rotation() const noexcept {
		return m_Rotation;
	}
	void Transform::Rotation(const glm::quat& _rotation) noexcept {
		m_Rotation = _rotation;
	}
	
	constexpr const glm::vec3& Transform::Scale() const noexcept {
		return m_Scale;
	}
	void Transform::Scale(const glm::vec3& _scale) noexcept {
		m_Scale = _scale;
	}
	
	glm::mat4 Transform::TRS() const {
		
		const auto& p = Position();
		const auto& r = Rotation();
		const auto& s = Scale();
		
		return glm::mat4 {
			s.x,   0,   0,   0,
			  0, s.y,   0,   0,
			  0,   0, s.z,   0,
			p.x, p.y, p.z,   1
		} * glm::mat4_cast(glm::inverse(r));
	}
	
} // LouiEriksson::Engine