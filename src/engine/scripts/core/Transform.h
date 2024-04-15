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
	
	/**
	 * @class Transform
	 * @brief Component representing a Transform.
	 *
	 * This class represents a Transform component, which is responsible for storing the position, rotation, and scale of an object in a 3D space. It inherits from the ECS::Component class.
	 */
	struct Transform final : public ECS::Component {
		
		glm::vec3 m_Position; /**< @brief Position of the Transform */
		glm::quat m_Rotation; /**< @brief Rotation of the Transform */
		glm::vec3 m_Scale;    /**< @brief    Scale of the Transform */
		
		explicit Transform(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Transform); };
		
		/**
		 * @brief Get the given vector (local to this transform) as it exists in world space.
		 * @param[in] _vector The vector to transform.
		 * @return The vector transformed to world space.
		 */
		[[nodiscard]] glm::vec3 ToWorld(const glm::vec3& _vector) const;
		
#define RIGHT   ToWorld(VEC_RIGHT)
#define UP      ToWorld(VEC_UP)
#define FORWARD ToWorld(VEC_FORWARD)
		
		/**
		 * @brief Get this Transform as a Transform, Rotation, Scale matrix.
		 * @return The TRS matrix of the Transform.
		 */
		[[nodiscard]] glm::mat4 TRS() const;
		
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_TRANSFORM_H