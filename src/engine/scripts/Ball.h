#pragma once

#ifndef FINALYEARPROJECT_BALL_H
#define FINALYEARPROJECT_BALL_H

#include "Scene.h"

namespace LouiEriksson {
	
	/// <summary>
	/// Ball object with Rigidbody physics.
	/// </summary>
	class Ball final : public Script {
	
		friend class Scene;
	
	protected:
	
		/// <summary> Mesh of the Ball. </summary>
		std::shared_ptr<Mesh> m_Mesh;
		
		/// <summary> Material of the Ball. </summary>
		std::shared_ptr<Material> m_Material;
	
		/// <summary> Called at the beginning of the first frame. </summary>
		void Begin() override;
	
		/// <summary> Called every frame. </summary>
		void Tick() override;
	
	public:
	
		explicit Ball(const std::shared_ptr<GameObject>& _parent);
		~Ball();
	
	};
}

#endif //FINALYEARPROJECT_BALL_H