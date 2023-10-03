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
	
		/// <summary> Material of the Ball. </summary>
		std::shared_ptr<Material> m_Material;
	
		/// <summary> Called at the beginning of the first frame. </summary>
		void Begin() override;
	
		/// <summary> Called every frame. </summary>
		void Tick() override;
	
	public:
	
		explicit Ball(std::weak_ptr<GameObject> _parent);
		~Ball() override;
	
	};
}

#endif //FINALYEARPROJECT_BALL_H