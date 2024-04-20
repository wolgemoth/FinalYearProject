#ifndef FINALYEARPROJECT_TRANSFORM_H
#define FINALYEARPROJECT_TRANSFORM_H

#include "../ecs/Component.h"
#include "../ecs/GameObject.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/fwd.hpp>

#include <memory>
#include <typeindex>

#define VEC_ZERO    glm::vec3(0)
#define VEC_ONE     glm::vec3(1)
#define VEC_RIGHT   glm::vec3(1, 0, 0)
#define VEC_UP      glm::vec3(0, 1, 0)
#define VEC_FORWARD glm::vec3(0, 0, 1)

#define QUAT_IDENTITY glm::quat(1, 0, 0, 0)

namespace LouiEriksson::Engine {
	
	/**
	 * @struct Transform
	 * @brief Component representing a Transform.
	 *
	 * This struct represents a Transform component, which is responsible for storing the position, rotation, and scale of an object in a 3D space. It inherits from the ECS::Component class.
	 */
	struct Transform final : public ECS::Component {
	
	private:
		
		glm::vec3 m_Position; /**< @brief Position of the Transform */
		glm::quat m_Rotation; /**< @brief Rotation of the Transform */
		glm::vec3 m_Scale;    /**< @brief    Scale of the Transform */
		
		enum Transformation : unsigned char {
			None       = 0u,
			Translated = 1u << 0u,
			Scaled     = 1u << 1u,
			Rotated    = 1u << 2u,
			Everything = (unsigned)Translated | (unsigned)Scaled | (unsigned)Rotated
		};
		
		Transformation m_Transformation;
		
		constexpr Transformation SetFlag(const Transformation& _other) const {
		    return static_cast<Transformation>(m_Transformation | static_cast<unsigned int>(_other));
		}
		
		[[nodiscard]] constexpr Transformation GetFlag(const Transformation& _other) const {
		    return static_cast<Transformation>(m_Transformation & static_cast<unsigned int>(_other));
		}
		
	public:
		
		explicit Transform(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : Component(_parent),
			m_Position(VEC_ZERO     ),
			m_Rotation(QUAT_IDENTITY),
			m_Scale   (VEC_ONE      ),
			m_Transformation(Transformation::Everything) { }
		
		/** @inheritdoc */
		[[nodiscard]] inline std::type_index TypeID() const noexcept override { return typeid(Transform); };
		
		/**
		 * @brief Get the given vector (local to this transform) as it exists in world space.
		 * @param[in] _vector The vector to transform.
		 * @return The vector transformed to world space.
		 */
		[[nodiscard]] constexpr glm::vec3 ToWorld(const glm::vec3& _vector) const {
			return _vector * m_Rotation;
		}
		
#define RIGHT   ToWorld(VEC_RIGHT)
#define UP      ToWorld(VEC_UP)
#define FORWARD ToWorld(VEC_FORWARD)
		
		[[nodiscard]] constexpr const glm::vec3& Position() const noexcept {
			return m_Position;
		}
		
		inline void Position(const glm::vec3& _position) noexcept {
			
			SetFlag(Translated);
			
			m_Position = _position;
		}
		
		[[nodiscard]] constexpr const glm::quat& Rotation() const noexcept {
			return m_Rotation;
		}
		
		inline void Rotation(const glm::quat& _rotation) noexcept {
			m_Rotation = _rotation;
		}
		
		[[nodiscard]] constexpr const glm::vec3& Scale() const noexcept {
			return m_Scale;
		}
		
		inline void Scale(const glm::vec3& _scale) noexcept  {
			m_Scale = _scale;
		}
		
		/**
		 * @brief Get this Transform as a Transform, Rotation, Scale matrix.
		 * @return The TRS matrix of the Transform.
		 */
		[[nodiscard]] inline glm::mat4 TRS() const {
			
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
		
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_TRANSFORM_H