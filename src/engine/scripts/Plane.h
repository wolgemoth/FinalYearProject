#ifndef FINALYEARPROJECT_PLANE_H
#define FINALYEARPROJECT_PLANE_H

#include "Scene.h"

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	/// <summary>
	/// A Plane with Collision.
	/// </summary>
	class Plane final : public Script {
	
		friend class Scene;
	
	protected:
	
		/// <summary> Mesh of the Plane. </summary>
		inline static std::shared_ptr<Mesh> m_Mesh = nullptr;
		
		/// <summary> Material of the Plane. </summary>
		inline static std::weak_ptr<Material> m_Material;
	
		/// <summary> Called at the beginning of every frame. </summary>
		void Begin() override;
	
		/// <summary> Called every frame. </summary>
		void Tick() override;
	
	public:
	
		explicit Plane(const std::shared_ptr<GameObject>& _parent);
		~Plane() override;
	
	};
}

#endif //FINALYEARPROJECT_PLANE_H