#ifndef FINALYEARPROJECT_PLANECOLLIDER_H
#define FINALYEARPROJECT_PLANECOLLIDER_H

#include "../Collider.h"

#include <memory>
#include <typeindex>

namespace LouiEriksson::Engine {
	
	class Transform;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::ECS {
	
	class GameObject;
	
} // LouiEriksson::Engine::ECS

namespace LouiEriksson::Engine::Physics {
	
	class PlaneCollider final : public Collider {
	
	public:
		
		explicit PlaneCollider(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		~PlaneCollider() override;
		
		[[nodiscard]] const std::type_index TypeID() const noexcept override { return typeid(PlaneCollider); };
		
		void SetTransform(const std::weak_ptr<Transform> &_transform) noexcept override;
		
	};
	
} // LouiEriksson::Engine::Physics

#endif //FINALYEARPROJECT_PLANECOLLIDER_H