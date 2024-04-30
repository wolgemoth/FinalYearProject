#ifndef FINALYEARPROJECT_TRANSFORM_HPP
#define FINALYEARPROJECT_TRANSFORM_HPP

#include "../ecs/Component.hpp"

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include <memory>
#include <typeindex>

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
		 * @brief Transform and scale components.
		 *
		 * Organised like this:
		 *     s.x,   0,   0,   0,
		 *       0, s.y,   0,   0,
		 *       0,   0, s.z,   0,
		 *     t.x, t.y, t.z,   1
		 */
		glm::mat4 m_TS;
		
		glm::mat4 m_World;
		glm::mat4 m_Local;
		
		/**< @brief Rotation of the Transform. */
		glm::quat m_Rotation;
		
		glm::vec3 m_LastPosition; /**< @brief Previous position of the Transform. */
		glm::quat m_LastRotation; /**< @brief Previous rotation of the Transform. */
		glm::vec3 m_LastScale;    /**< @brief Previous scale of the Transform.    */
		
		/** @brief Parent of this Transform. */
		std::weak_ptr<Transform> m_Parent;
		
		/** @brief Children of this Transform. */
		std::vector<std::weak_ptr<Transform>> m_Children;
		
	public:
		
		explicit Transform(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : Component(_parent),
			          m_TS(1.0),
			       m_World(1.0),
			       m_Local(1.0),
			    m_Rotation(QUAT_IDENTITY),
			m_LastPosition(0.0),
			m_LastRotation(QUAT_IDENTITY),
			m_LastScale   (1.0) {}
		
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Transform); };
		
		void ParentTransform(const std::weak_ptr<Transform>& _value) {
			m_Parent = _value;
		}
		
		[[nodiscard]] std::shared_ptr<Transform> ParentTransform() {
			
			std::shared_ptr<Transform> result;
			
			if (const auto p = m_Parent.lock()) {
				result = p;
			}
			
			return result;
		}
		
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
		 * @brief Returns the model / World matrix of this transform.
		 * @return The World matrix of the Transform.
		 */
		[[nodiscard]] const glm::mat4& Local() {
		
			if (m_Rotation != QUAT_IDENTITY && m_Rotation != QUAT_IDENTITY_NEG) {
				
				if (Position() != m_LastPosition ||
				    m_Rotation != m_LastRotation ||
					   Scale() != m_LastScale
				) {
					m_LastPosition = Position();
					m_LastRotation = Rotation();
					m_LastScale    = Scale();
					
					m_Local = m_TS * glm::mat4_cast(glm::inverse(m_Rotation));
				}
			}
			else {
				m_Local = m_TS;
			}
			
			return m_Local;
		}
		
		[[nodiscard]] glm::mat4 World() {
			
			glm::mat4 result;
			
			if (const auto p = m_Parent.lock()) {
				m_World = p->World() * Local();
				
				result = m_World;
			}
			else {
				result = Local();
			}
			
			return result;
		}
		
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_TRANSFORM_HPP