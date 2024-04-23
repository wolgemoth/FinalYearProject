#ifndef FINALYEARPROJECT_TRANSFORM_HPP
#define FINALYEARPROJECT_TRANSFORM_HPP

#include "../ecs/Component.hpp"

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include <memory>
#include <typeindex>
#include <optional>

#define VEC_ZERO    glm::vec3(0)
#define VEC_ONE     glm::vec3(1)
#define VEC_RIGHT   glm::vec3(1, 0, 0)
#define VEC_UP      glm::vec3(0, 1, 0)
#define VEC_FORWARD glm::vec3(0, 0, 1)

#define QUAT_IDENTITY     glm::quat( 1, 0, 0, 0)
#define QUAT_IDENTITY_NEG glm::quat(-1, 0, 0, 0)

namespace LouiEriksson::Engine::Graphics {
	
	class Camera;
}

namespace LouiEriksson::Engine {
	
	/**
	 * @struct Transform
	 * @brief Component representing a Transform.
	 *
	 * This struct represents a Transform component, which is responsible for storing the position, rotation, and scale of an object in a 3D space. It inherits from the ECS::Component class.
	 */
	struct Transform final : public ECS::Component {
	
	private:
		
		/**
		 * @brief Position and scale of the transform.
		 *
		 * Organised like this:
		 *     s.x,   0,   0,   0,
		 *       0, s.y,   0,   0,
		 *       0,   0, s.z,   0,
		 *     t.x, t.y, t.z,   1
		 */
		glm::mat4 m_TS;
		glm::mat4 m_TRS;
		
		glm::vec3 m_LastPosition; /**< @brief Previous position of the Transform. */
		glm::quat m_LastRotation; /**< @brief Previous rotation of the Transform. */
		glm::vec3 m_LastScale;    /**< @brief Previous scale of the Transform. */
		
		glm::quat m_Rotation; /**< @brief 'Pending' rotation of the Transform. */
		
	public:
		
		explicit Transform(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : Component(_parent),
			          m_TS(1.0),
			         m_TRS(1.0),
			m_LastRotation(QUAT_IDENTITY),
			    m_Rotation(QUAT_IDENTITY) {}
		
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Transform); };
		
		/**
		 * @brief Get the given vector (local to this transform) as it exists in world space.
		 * @param[in] _vector The vector to transform.
		 * @return The vector transformed to world space.
		 */
		[[nodiscard]] glm::vec3 ToWorld(const glm::vec3& _vector) const {
			return _vector * m_Rotation;
		}
		
#define RIGHT   ToWorld(VEC_RIGHT)
#define UP      ToWorld(VEC_UP)
#define FORWARD ToWorld(VEC_FORWARD)
		
		[[nodiscard]] glm::vec3 Position() const noexcept {
			return m_TS[3];
		}
		
		void Position(const glm::vec3& _position) noexcept {
			m_TS[3] = glm::vec4(_position, 1.0);
		}
		
		[[nodiscard]] constexpr const glm::quat& Rotation() const noexcept {
			return m_Rotation;
		}
		
		void Rotation(const glm::quat& _rotation) noexcept {
			m_Rotation = _rotation;
		}
		
		[[nodiscard]] glm::vec3 Scale() const noexcept {
			return { m_TS[0].x, m_TS[1].y, m_TS[2].z };
		}
		
		void Scale(const glm::vec3& _scale) noexcept  {
			m_TS[0].x = _scale.x;
			m_TS[1].y = _scale.y;
			m_TS[2].z = _scale.z;
		}
		
		/**
		 * @brief Returns the model / TRS matrix of this transform.
		 * @return The TRS matrix of the Transform.
		 */
		[[nodiscard]] const glm::mat4& TRS() {
			return m_TRS;
		}
		
		void RecalculateTRS() {
			
			if (m_Rotation != QUAT_IDENTITY && m_Rotation != QUAT_IDENTITY_NEG) {
				
				if (Position() != m_LastPosition ||
				    m_Rotation != m_LastRotation ||
					   Scale() != m_LastScale
				) {
					m_LastPosition = Position();
					m_LastRotation = Rotation();
					m_LastScale    = Scale();
					
					m_TRS = m_TS * glm::mat4_cast(glm::inverse(m_Rotation));
				}
			}
			else {
				m_TRS = m_TS;
			}
		}
		
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_TRANSFORM_HPP