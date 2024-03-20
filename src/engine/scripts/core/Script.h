#ifndef FINALYEARPROJECT_SCRIPT_H
#define FINALYEARPROJECT_SCRIPT_H

#include "../ecs/Component.h"

#include <memory>

namespace LouiEriksson::Engine::ECS {
	
	class Scene;
	
} // LouiEriksson::Engine::ECS

namespace LouiEriksson::Engine::Physics {
	
	class Camera;
	class Collision;
	
} // LouiEriksson::Engine::Physics

namespace LouiEriksson::Game::Core {
	
	class ScriptInjector;
	
} // LouiEriksson::Game::Core

namespace LouiEriksson::Engine {

	/// <summary>
	/// Base class to be inherited by scriptable types.
	/// </summary>
	class Script : public ECS::Component {
	
		friend ECS::Scene;
		friend LouiEriksson::Game::Core::ScriptInjector;
	
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
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_SCRIPT_H