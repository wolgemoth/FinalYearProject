#ifndef FINALYEARPROJECT_TRANSFORM_H
#define FINALYEARPROJECT_TRANSFORM_H

#include "../ecs/Component.h"

#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>

#include <memory>
#include <typeindex>

#define VEC_ZERO    glm::vec3(0)
#define VEC_ONE     glm::vec3(1)
#define VEC_RIGHT   glm::vec3(1, 0, 0)
#define VEC_UP      glm::vec3(0, 1, 0)
#define VEC_FORWARD glm::vec3(0, 0, 1)

#define QUAT_IDENTITY glm::quat(1.0f, 0.0f, 0.0f, 0.0f)

namespace LouiEriksson::Engine {

	/// <summary>
	/// Component representing a Transform.
	/// </summary>
	struct Transform final : public ECS::Component {
		
		/// <summary> Position of the Transform. </summary>
		glm::vec3 m_Position;
		
		/// <summary> Rotation of the Transform. </summary>
		glm::quat m_Rotation;
		
		/// <summary> Scale of the Transform. </summary>
		glm::vec3 m_Scale;
		
		explicit Transform(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Transform); };
		
		/// <summary> Get the given vector (local to this transform) as it exists in world space.</summary>
		[[nodiscard]] glm::vec3 ToWorld(const glm::vec3& _vector) const;
		
#define RIGHT   ToWorld(VEC_RIGHT)
#define UP      ToWorld(VEC_UP)
#define FORWARD ToWorld(VEC_FORWARD)
		
		/// <summary> Get this Transform as a Transform, Rotation, Scale matrix.</summary>
		[[nodiscard]] glm::mat4 TRS() const;
		
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_TRANSFORM_H