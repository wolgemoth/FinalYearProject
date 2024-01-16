#ifndef FINALYEARPROJECT_SPHERECOLLIDER_H
#define FINALYEARPROJECT_SPHERECOLLIDER_H

#include "../Collider.h"

#include <memory>

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson::Engine::ECS {
	
	class GameObject;
	
} // LouiEriksson::Engine::ECS

namespace LouiEriksson::Engine::Physics {
	
	class SphereCollider final : public Collider {
	
	public:
		
		explicit SphereCollider(const std::weak_ptr<ECS::GameObject>& _parent);
		~SphereCollider() override;
		
		std::type_index TypeID() const noexcept override { return typeid(SphereCollider); };
		
		/// <summary> Set the radius of the SphereCollider. </summary>
		void Radius(const float& _radius);
		
		/// <summary> Get the radius of the SphereCollider. </summary>
		[[nodiscard]] float Radius() const;
		
	};
	
} // LouiEriksson::Engine::Physics

#endif //FINALYEARPROJECT_SPHERECOLLIDER_H