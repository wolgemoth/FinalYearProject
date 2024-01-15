#ifndef FINALYEARPROJECT_SCRIPT_H
#define FINALYEARPROJECT_SCRIPT_H

#include "../ecs/Component.h"

#include <memory>
#include <typeindex>

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson::ECS {
	
	class Scene;
	
} // LouiEriksson::ECS

namespace LouiEriksson::Physics {
	
	class Camera;
	class Collision;
	
} // LouiEriksson::Physics

namespace LouiEriksson::Scripts {
	
	class ScriptInjector;
	
} // LouiEriksson::Scripts

namespace LouiEriksson {

	/// <summary>
	/// Base class to be inherited by scriptable types.
	/// </summary>
	class Script : public ECS::Component {
	
		friend ECS::Scene;
		friend Scripts::ScriptInjector;
	
	protected:
	
		explicit Script(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		~Script() override = default;
	
		/// <summary> Called at the beginning of the first frame. </summary>
		virtual void Begin();
	
		/// <summary> Called every frame. </summary>
		virtual void Tick();
		
		/// <summary> Called every tick of the physics engine. </summary>
		virtual void FixedTick();
		
		/// <summary> Called every time a collision event occurs. </summary>
		virtual void OnCollision(const Physics::Collision& _collision);
		
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_SCRIPT_H