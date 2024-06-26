#ifndef FINALYEARPROJECT_SCRIPT_HPP
#define FINALYEARPROJECT_SCRIPT_HPP

#include "../ecs/Component.hpp"

#include <memory>

namespace LouiEriksson::Engine::ECS {
	
	class GameObject;
	class Scene;
	
} // LouiEriksson::Engine::ECS

namespace LouiEriksson::Engine::Physics {
	
	struct Collision;
	
} // LouiEriksson::Engine::Physics

namespace LouiEriksson::Game::Core {
	
	class ScriptInjector;
	
} // LouiEriksson::Game::Core

namespace LouiEriksson::Engine {
	
	/**
	 * @class Script
	 * @brief Base class to be inherited by scriptable types.
	 * @details Script is a base class for scriptable types which can be attached to a Parent.
	 * It inherits from the Component class and provides virtual functions that can be overridden by derived classes.
	 *
	 * @see Component
	 * @see ECS::Scene
	 */
	class Script : public ECS::Component {
	
		friend ECS::Scene;
		friend LouiEriksson::Game::Core::ScriptInjector;
	
	private:
		
		bool m_Initialised = false;
		
		void Invoke(const bool& _late) {
			
			try {
				
				if (!_late) {
					
					if (!m_Initialised) {
						Begin();
						m_Initialised = true;
					}
					else {
						Tick();
					}
				}
				else if (m_Initialised) {
					LateTick();
				}
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
		}
		
	protected:
		
		/**
		 * @brief Construct a new Script object and bind to a Parent.
		 *
		 * @param[in] _parent A weak pointer to the parent ECS::Parent that the
		 *                    Script will be associated with.
		 */
		explicit Script(const std::weak_ptr<ECS::GameObject>& _parent) : ECS::Component(_parent) {}
		
		/** @inheritdoc */
		virtual ~Script() override = default;
	
		/** @brief Called at the beginning of the first frame. */
		virtual void Begin() {}
		
		/** @brief Called at the beginning of every frame. */
		virtual void Tick() {}
		
		/** @brief Called at the end of every frame. */
		virtual void LateTick() {}
		
		/** @brief Called every tick of the physics engine. */
		virtual void FixedTick() {}
		
		/**
		 * @brief Called every time a collision event occurs.
		 *
		 * This function is called whenever a collision event occurs in the physics engine.
		 *
		 * @param[in] _collision A constant reference to a Collision object representing the collision event.
		 * @see Physics::Collision
		 */
		virtual void OnCollision(const Physics::Collision& _collision) {}
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_SCRIPT_HPP