#ifndef _PLANECOLLIDER_H
#define _PLANECOLLIDER_H

#include "Collider.h"

class PlaneCollider final : Collider {

public:
	
	PlaneCollider(std::weak_ptr<GameObject> _parent);
	
	~PlaneCollider();
	
	float DistanceToPlane(glm::vec3 _point, glm::vec3 _pointOnPlane);
	
	bool Evaluate(std::shared_ptr<Collider> _other);
	
	glm::vec3 Normal();
};

#endif //_PLANECOLLIDER_H
