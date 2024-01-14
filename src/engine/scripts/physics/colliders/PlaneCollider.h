#ifndef FINALYEARPROJECT_PLANECOLLIDER_H
#define FINALYEARPROJECT_PLANECOLLIDER_H

#include "../Collider.h"

#include <memory>

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	class Transform;
	
} // LouiEriksson

namespace LouiEriksson::ECS {
	
	class GameObject;
	
} // LouiEriksson::ECS

namespace LouiEriksson::Physics {
	
	class PlaneCollider final : public Collider {
	
	public:
		
		explicit PlaneCollider(const std::shared_ptr<ECS::GameObject>& _parent) noexcept;
		~PlaneCollider() override;
		
		void SetTransform(const std::weak_ptr<Transform> &_transform) noexcept override;
		
	};
	
} // LouiEriksson::Physics

#endif //FINALYEARPROJECT_PLANECOLLIDER_H