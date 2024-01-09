#ifndef FINALYEARPROJECT_RIGIDBODY_H
#define FINALYEARPROJECT_RIGIDBODY_H

#include "Transform.h"
#include "Collision.h"

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	class Collider;
	
	/// <summary>
	/// Rigidbody Component implementing Rigidbody dynamics and Collision response behaviour.
	/// </summary>
	class Rigidbody : public Component {
	
	private:
		
		struct Parameters;
		
		struct BulletRigidbody {
			
			friend Parameters;
			
			/// <summary>
			/// Implementation of the abstract btCollisionWorld::ContactResultCallback class.
			/// </summary>
			class CollisionCallback : public btCollisionWorld::ContactResultCallback {
			
				/// <summary> Reference to the rigidbody this object belongs to. </summary>
				std::weak_ptr<Rigidbody> m_Rigidbody;
				
				/// </inheritdoc>
			    btScalar addSingleResult(btManifoldPoint& _cp, const btCollisionObjectWrapper* _colObj0Wrap, int _partId0, int _index0, const btCollisionObjectWrapper* _colObj1Wrap, int _partId1, int _index1) override;
				
				 CollisionCallback(std::weak_ptr<Rigidbody> _rigidbody);
				~CollisionCallback()
			};
			
			/// <summary> (Bullet Physics Engine) Internal Rigidbody component. </summary>
			std::shared_ptr<btRigidBody> m_Rigidbody;
			
			/// <summary> (Bullet Physics Engine) Internal Rigidbody component. </summary>
			std::shared_ptr<btMotionState> m_MotionState;
			
			/// <summary> Reference to collision callback. </summary>
			std::weak_ptr<CollisionCallback> m_Callback;
			
			 BulletRigidbody(const std::weak_ptr<Rigidbody>& _rigidbody, const std::weak_ptr<Transform>& _transform, const std::weak_ptr<Collider>& _collider, const Parameters& _parameters);
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
			
			Parameters();
			
		};
		
		/// <summary> Instance of this Rigidbody's parameters. </summary>
		Parameters m_Parameters;
		
		/// <summary> Transform of the Rigidbody </summary>
		std::weak_ptr<Transform> m_Transform;
		
		/// <summary> Collider of the Rigidbody. </summary>
		std::weak_ptr<Collider> m_Collider;
	
		void BulletReinitialise();
		
	public:
	
		explicit Rigidbody(const std::shared_ptr<GameObject>& _parent);
		~Rigidbody() override = default;
	
		/// <summary> Called every frame. </summary>
		void Interpolate();
		
		/// <summary> Sync the transform with the physics engine. </summary>
		void Sync();
		
		/// <summary> Invoked when this Rigidbody collides with something. </summary>
		void OnCollision(Collision& _collision);
		
		/// <summary> Set the Transform of the Rigidbody. </summary>
		void SetTransform(const std::weak_ptr<Transform>& _transform);
	
		/// <summary> Get the Transform of the Rigidbody. </summary>
		const std::weak_ptr<Transform>& GetTransform() const;
	
		/// <summary> Set the Collider of the Rigidbody. </summary>
		void SetCollider(const std::weak_ptr<Collider>& _collider);
	
		/// <summary> Get the Collider of the Rigidbody. </summary>
		const std::weak_ptr<Collider>& GetCollider() const ;
	
		/// <summary>
		/// Set the position of the Rigidbody.
		/// You shouldn't normally need to do this.
		/// See AddForce() Instead.
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
		[[nodiscard]] const bool& Kinematic() const;
		
		/// <summary> Set the kinematic state of the Rigidbody. </summary>
		void Gravity(const bool& _value);
		
		/// <summary> Get the kinematic state of the Rigidbody. </summary>
		[[nodiscard]] const bool& Gravity() const;
		
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
		[[nodiscard]] const float& Mass() const;
	
		/// <summary> Set the drag of the Rigidbody. </summary>
		void Drag(const float& _value);
	
		/// <summary> Get the drag of the Rigidbody. </summary>
		[[nodiscard]] const float& Drag() const;
	
		/// <summary> Set the angular drag of the Rigidbody. </summary>
		void AngularDrag(const float& _value);
	
		/// <summary> Get the angular drag of the Rigidbody. </summary>
		[[nodiscard]] const float& AngularDrag() const;
	
		/// <summary> Set the friction of the Rigidbody. </summary>
		void Friction(const float& _value);
	
		/// <summary> Get the friction of the Rigidbody. </summary>
		[[nodiscard]] const float& Friction() const;
		
		/// <summary> Set the bounciness of the Rigidbody. </summary>
		void Bounciness(const float& _value);
	
		/// <summary> Get the bounciness of the Rigidbody. </summary>
		[[nodiscard]] const float& Bounciness() const;
	};
}

#endif //FINALYEARPROJECT_RIGIDBODY_H