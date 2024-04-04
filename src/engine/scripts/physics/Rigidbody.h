#ifndef FINALYEARPROJECT_RIGIDBODY_H
#define FINALYEARPROJECT_RIGIDBODY_H

#include "../ecs/Component.h"

#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float3.hpp>
#include <LinearMath/btMotionState.h>
#include <LinearMath/btVector3.h>

#include <memory>
#include <typeindex>
#include <vector>

namespace LouiEriksson::Engine {
	
	class Transform;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Physics {
	
	class Collider;
	class Collision;
	
	/// <summary>
	/// Rigidbody Component implementing Rigidbody dynamics and Collision response behaviour.
	/// </summary>
	class Rigidbody final : public ECS::Component {
	
	private:
		
		struct Parameters;
		
		class BulletRigidbody {
			
			friend Parameters;
			
		public:
			
			/// <summary> (Bullet Physics Engine) Internal Rigidbody component. </summary>
			std::shared_ptr<btRigidBody> m_Rigidbody;
			
			/// <summary> (Bullet Physics Engine) Internal Rigidbody component. </summary>
			std::shared_ptr<btMotionState> m_MotionState;
			
			 BulletRigidbody(const std::weak_ptr<Transform>& _transform, const std::weak_ptr<Collider>& _collider, const Parameters& _parameters);
			~BulletRigidbody();
			
		};
		
		/// <summary> Parameters of a Rigidbody. </summary>
		struct Parameters {
		
			friend Rigidbody;
			
		private:
			
			// <summary> Wrapped implementation of Bullet's btRigidBody. </summary>
			std::shared_ptr<BulletRigidbody> m_BulletRigidbody;
			
			/// <summary> Whether or not the Rigidbody is influenced by external forces. </summary>
			bool m_Kinematic;
			
			/// <summary> Whether or not the Rigidbody is affected by gravity. </summary>
			bool m_UseGravity;
			
			/// <summary> Whether or not the Rigidbody should use continuous collision detection (CCD). </summary>
			bool m_Continuous;
			
			/// <summary> Mass of the Rigidbody. </summary>
			float m_Mass;
			
			/// <summary> Drag of the Rigidbody. </summary>
			float m_Drag;
		
			/// <summary> Angular drag of the Rigidbody. </summary>
			float m_AngularDrag;
		
			/// <summary> Friction of the Rigidbody. </summary>
			float m_Friction;
			
			/// <summary> Bounciness of the Rigidbody. </summary>
			float m_Bounciness;
			
			/// <summary> Resistance of object to changes in angular velocity. </summary>
			btVector3 m_Inertia;
			
		public:
			
			Parameters() noexcept;
			
		};
		
		/// <summary> Instance of this Rigidbody's parameters. </summary>
		Parameters m_Parameters;
		
		/// <summary> Transform of the Rigidbody </summary>
		std::weak_ptr<Transform> m_Transform;
		
		/// <summary> Collider of the Rigidbody. </summary>
		std::weak_ptr<Collider> m_Collider;
	
		/// <summary> List of collisions for the current tick of the physics engine. </summary>
		std::vector<Collision> m_Collisions;
		
		void BulletReinitialise();
		
	public:
	
		explicit Rigidbody(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		~Rigidbody() override = default;
	
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Rigidbody); };
		
		/// <summary> Called every frame. </summary>
		void Interpolate();
		
		/// <summary> Sync the transform with the physics engine. </summary>
		void Sync();
		
		/// <summary> Retrieve the list of collisions for the current tick of the physics engine. </summary>
		[[nodiscard]] const std::vector<Collision>& Collisions() const noexcept;
		
		/// <summary> Set the Transform of the Rigidbody. </summary>
		void SetTransform(const std::weak_ptr<Transform>& _transform);
	
		/// <summary> Get the Transform of the Rigidbody. </summary>
		[[nodiscard]] const std::weak_ptr<Transform>& GetTransform() const noexcept;
	
		/// <summary> Set the Collider of the Rigidbody. </summary>
		void SetCollider(const std::weak_ptr<Collider>& _collider);
	
		/// <summary> Get the Collider of the Rigidbody. </summary>
		[[nodiscard]] const std::weak_ptr<Collider>& GetCollider() const noexcept;
	
		/// <summary>
		/// Set the position of the Rigidbody.
		/// You shouldn't normally need to do this.
		/// See AddForce() instead.
		/// </summary>
		void Position(const glm::vec3& _value);
		
		/// <summary> Get the position of the Rigidbody. </summary>
		const glm::vec3& Position();
		
		/// <summary>
		/// Set the rotation of the Rigidbody.
		/// You shouldn't normally need to do this.
		/// See AddForce() Instead.
		/// </summary>
		void Rotation(const glm::quat& _value);
		
		/// <summary> Get the rotation of the Rigidbody. </summary>
		const glm::quat& Rotation();
		
		/// <summary> Set the kinematic state of the Rigidbody. </summary>
		void Kinematic(const bool& _value);
		
		/// <summary> Get the kinematic state of the Rigidbody. </summary>
		[[nodiscard]] const bool& Kinematic() const noexcept;
		
		/// <summary> Set the kinematic state of the Rigidbody. </summary>
		void Gravity(const bool& _value);
		
		/// <summary> Get the kinematic state of the Rigidbody. </summary>
		[[nodiscard]] const bool& Gravity() const noexcept;
		
		/// <summary> Set the velocity of the Rigidbody. </summary>
		void Velocity(const glm::vec3& _value);
		
		/// <summary> Get the velocity of the Rigidbody. </summary>
		[[nodiscard]] glm::vec3 Velocity() const;
		
		/// <summary> Set the angular velocity of the Rigidbody. </summary>
		void AngularVelocity(const glm::vec3& _value);
	
		/// <summary> Get the angular velocity of the Rigidbody. </summary>
		[[nodiscard]] glm::vec3 AngularVelocity() const;
	
		/// <summary> Add a force to the Rigidbody. </summary>
		void AddForce(const glm::vec3& _value, const glm::vec3& _relativePosition = glm::vec3(0.0f));
	
		/// <summary> Get the force of the Rigidbody. </summary>
		[[nodiscard]] glm::vec3 GetForce() const;
	
		/// <summary> Set the mass of the Rigidbody. </summary>
		void Mass(const float& _value);
		
		/// <summary> Get the mass of the Rigidbody. </summary>
		[[nodiscard]] const float& Mass() const noexcept;
	
		/// <summary> Set the drag of the Rigidbody. </summary>
		void Drag(const float& _value);
	
		/// <summary> Get the drag of the Rigidbody. </summary>
		[[nodiscard]] const float& Drag() const noexcept;
	
		/// <summary> Set the angular drag of the Rigidbody. </summary>
		void AngularDrag(const float& _value);
	
		/// <summary> Get the angular drag of the Rigidbody. </summary>
		[[nodiscard]] const float& AngularDrag() const noexcept;
	
		/// <summary> Set the friction of the Rigidbody. </summary>
		void Friction(const float& _value);
	
		/// <summary> Get the friction of the Rigidbody. </summary>
		[[nodiscard]] const float& Friction() const noexcept;
		
		/// <summary> Set the bounciness of the Rigidbody. </summary>
		void Bounciness(const float& _value);
	
		/// <summary> Get the bounciness of the Rigidbody. </summary>
		[[nodiscard]] const float& Bounciness() const noexcept;
		
	};
	
} // LouiEriksson::Engine::Physics

#endif //FINALYEARPROJECT_RIGIDBODY_H