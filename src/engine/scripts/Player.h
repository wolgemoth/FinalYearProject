#pragma once

#ifndef FINALYEARPROJECT_PLAYER_H
#define FINALYEARPROJECT_PLAYER_H

#include "Scene.h"

namespace LouiEriksson {
	
	class Player final : public Script {
	
		friend class Scene;
	
	private:
	
		/// <summary> Test model m_Curuthers. </summary>
		std::weak_ptr<GameObject> m_Curuthers;
	
		/// <summary> Player Camera. </summary>
		std::weak_ptr<Camera> m_Camera;
	
	protected:
	
		/// <summary> Called at the beginning of the first frame. </summary>
		void Begin() override;
	
		/// <summary> Called every frame. </summary>
		void Tick() override;
	
	public:
	
		 explicit Player(std::weak_ptr<GameObject> _parent);
		~Player() override;
	
	};
}

#endif //FINALYEARPROJECT_PLAYER_H