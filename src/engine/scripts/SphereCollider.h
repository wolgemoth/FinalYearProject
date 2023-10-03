#ifndef FINALYEARPROJECT_SPHERECOLLIDER_H
#define FINALYEARPROJECT_SPHERECOLLIDER_H

#include "Collider.h"

namespace LouiEriksson {
	
	class SphereCollider final : Collider {
	
	private:
		
		float m_Radius;
		
	public:
		
		explicit SphereCollider(std::weak_ptr<GameObject> _parent);
		~SphereCollider() override;
		
		void  Radius(const float& _radius) override;
		float Radius()                     override;
		
	};
}

#endif //FINALYEARPROJECT_SPHERECOLLIDER_H