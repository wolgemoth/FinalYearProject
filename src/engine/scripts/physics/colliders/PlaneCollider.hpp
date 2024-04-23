#ifndef FINALYEARPROJECT_PLANECOLLIDER_HPP
#define FINALYEARPROJECT_PLANECOLLIDER_HPP

#include "../../core/Debug.hpp"
#include "../../core/Transform.hpp"
#include "../Collider.hpp"

#include <BulletCollision/CollisionShapes/btBoxShape.h>

#include <memory>
#include <typeindex>

namespace LouiEriksson::Engine::ECS {
	
	class GameObject;
	
} // LouiEriksson::Engine::ECS

namespace LouiEriksson::Engine::Physics {
	
	class PlaneCollider final : public Collider {
	
	public:
		
		explicit PlaneCollider(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : Collider(_parent) {};
		
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(PlaneCollider); };
		
		/** @inheritdoc */
		void SetTransform(const std::weak_ptr<Transform> &_transform) noexcept override {
			
			Collider::SetTransform(_transform); // Call base function to assign new transform.
			
			// Lock the transform and check it is valid.
			if (const auto t = m_Transform.lock()) {
				
				const auto s = t->Scale();
				
				/* Use box-shaped collision instead of a plane-shape so that
				 * we can allow our plane to have edges, and not just be infinite. */
				try {
					m_CollisionShape.reset(
						new btBoxShape({
							static_cast<btScalar>(s.x / 2.0),
							0.0001,
							static_cast<btScalar>(s.z / 2.0)
						})
					);
				}
				catch (const std::exception& e) {
					Debug::Log(e);
				}
			}
		}
		
	};
	
} // LouiEriksson::Engine::Physics

#endif //FINALYEARPROJECT_PLANECOLLIDER_HPP