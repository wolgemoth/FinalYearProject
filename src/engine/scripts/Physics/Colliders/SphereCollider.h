#ifndef FINALYEARPROJECT_SPHERECOLLIDER_H
#define FINALYEARPROJECT_SPHERECOLLIDER_H

#include "../Collider.h"

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	class SphereCollider final : public Collider {
	
	public:
		
		explicit SphereCollider(const std::shared_ptr<GameObject>& _parent);
		~SphereCollider() override;
		
		/// <summary> Set the radius of the SphereCollider. </summary>
		void Radius(const float& _radius);
		
		/// <summary> Get the radius of the SphereCollider. </summary>
		[[nodiscard]] float Radius() const;
		
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_SPHERECOLLIDER_H