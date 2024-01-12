#ifndef FINALYEARPROJECT_SCRIPT_H
#define FINALYEARPROJECT_SCRIPT_H

#include "../ecs/Component.h"

#include <memory>

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.


namespace LouiEriksson {

	class Collision;
	class Scene;
	
	/// <summary>
	/// Base class to be inherited by scriptable types.
	/// </summary>
	class Script : public Component {
	
		friend class Scene;
	
	protected:
	
		explicit Script(const std::shared_ptr<GameObject>& _parent);
		~Script() override = default;
	
		/// <summary> Called at the beginning of the first frame. </summary>
		virtual void Begin();
	
		/// <summary> Called every frame. </summary>
		virtual void Tick();
		
		/// <summary> Called every tick of the physics engine. </summary>
		virtual void FixedTick();
		
		/// <summary> Called every time a collision event occurs. </summary>
		virtual void OnCollision(const Collision& _collision);
	};
}

#endif //FINALYEARPROJECT_SCRIPT_H