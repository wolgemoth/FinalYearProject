#ifndef FINALYEARPROJECT_PLANECOLLIDER_H
#define FINALYEARPROJECT_PLANECOLLIDER_H

#include "Collider.h"

namespace LouiEriksson {
	
	class PlaneCollider final : Collider {
	
	public:
		
		explicit PlaneCollider(const std::shared_ptr<GameObject>& _parent);
		
		~PlaneCollider() override;
		
		float DistanceToPlane(glm::vec3 _point, glm::vec3 _pointOnPlane);
		
		bool Evaluate(const std::shared_ptr<Collider>& _other);
		
		glm::vec3 Normal();
	};
}

#endif //FINALYEARPROJECT_PLANECOLLIDER_H