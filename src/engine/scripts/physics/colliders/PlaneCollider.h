#ifndef FINALYEARPROJECT_PLANECOLLIDER_H
#define FINALYEARPROJECT_PLANECOLLIDER_H

#include "../../core/Transform.h"
#include "../../ecs/GameObject.h"
#include "../Collider.h"

#include <memory>

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	class PlaneCollider final : public Collider {
	
	public:
		
		explicit PlaneCollider(const std::shared_ptr<GameObject>& _parent);
		~PlaneCollider() override;
		
		void SetTransform(const std::weak_ptr<Transform> &_transform) override;
		
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_PLANECOLLIDER_H