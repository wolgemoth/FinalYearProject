#ifndef _SPHERECOLLIDER_H
#define _SPHERECOLLIDER_H

#include "Collider.h"

class SphereCollider final : Collider {

private:
	
	float m_Radius;
	
public:
	
	 SphereCollider(std::weak_ptr<GameObject> _parent);
	~SphereCollider();
	
	void Radius(const float &_radius);
	
	float Radius();
	
};

#endif //_SPHERECOLLIDER_H
