#pragma once

#ifndef FINALYEARPROJECT_PLANE_H
#define FINALYEARPROJECT_PLANE_H

#include "Scene.h"

namespace LouiEriksson {
	
	/// <summary>
	/// A Plane with Collision.
	/// </summary>
	class Plane final : public Script {
	
		friend class Scene;
	
	protected:
	
		/// <summary> Material of the Plane. </summary>
		std::shared_ptr<Material> m_Material;
	
		/// <summary> Called at the beginning of every frame. </summary>
		void Begin() override;
	
		/// <summary> Called every frame. </summary>
		void Tick() override;
	
	public:
	
		explicit Plane(std::weak_ptr<GameObject> _parent);
		~Plane() override;
	
	};
}

#endif //FINALYEARPROJECT_PLANE_H