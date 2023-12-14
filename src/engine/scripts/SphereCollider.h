#ifndef FINALYEARPROJECT_SPHERECOLLIDER_H
#define FINALYEARPROJECT_SPHERECOLLIDER_H

#include "Collider.h"

namespace LouiEriksson {
	
	class SphereCollider final : Collider {
	
	private:
		
		float m_Radius;
		
	public:
		
		explicit SphereCollider(const std::shared_ptr<GameObject>& _parent);
		~SphereCollider() override;
		
		void  Radius(const float& _radius) override;
		[[nodiscard]] float Radius() const;
		
	};
}

#endif //FINALYEARPROJECT_SPHERECOLLIDER_H