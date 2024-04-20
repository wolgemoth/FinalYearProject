#ifndef FINALYEARPROJECT_PHYSICS_H
#define FINALYEARPROJECT_PHYSICS_H

#include "../core/Defaults.h"

#include "LinearMath/btScalar.h"
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <glm/ext/vector_float3.hpp>
#include <LinearMath/btIDebugDraw.h>
#include <LinearMath/btVector3.h>

#include <memory>

namespace LouiEriksson::Engine {
	
	class Application;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Physics {
	
	/**
	 * @class Physics
	 * @brief Manages the physics engine of the Application.
	 */
	class Physics final {
	
		friend LouiEriksson::Engine::Application;
		
		friend class Rigidbody;
		
	private:
		
		/**
		 * @class Debugger
		 * @brief Implementation of the abstract btIDebugDraw class.
		 */
		class Debugger final : public btIDebugDraw {
		
		private:

			/** @brief Weak pointer to the btDynamicsWorld linked to this debugger. */
			std::weak_ptr<btDynamicsWorld> m_DynamicsWorld;
			
		public:
			
			 explicit Debugger(const std::weak_ptr<btDynamicsWorld>& _dynamicsWorld);
			 
			/** @inheritdoc */
			~Debugger() override;
			
			/** @inheritdoc */
			void drawLine(const btVector3 &_from, const btVector3 &_to, const btVector3 &_color) override;
			
			/** @inheritdoc */
			void drawContactPoint(const btVector3 &_PointOnB, const btVector3 &_normalOnB, btScalar _distance, int _lifeTime, const btVector3 &_color) override;
			
			/** @inheritdoc */
			void reportErrorWarning(const char *_warningString) override;
			
			/** @inheritdoc */
			void draw3dText(const btVector3 &_location, const char *_textString) override;
			
			/** @inheritdoc */
			void setDebugMode(int _debugMode) override;
			
			/** @inheritdoc */
			[[nodiscard]] int getDebugMode() const override;
			
		};
		
		/**< @brief Instance of class used to visualise the state of the physics engine for debugging purposes. */
		inline static std::shared_ptr<Debugger> s_Debugger;
		
		inline static std::shared_ptr<btDefaultCollisionConfiguration>     s_Configuration; /**< @brief (Bullet Physics) Configuration of the simulation. */
		inline static std::shared_ptr<btCollisionDispatcher>                  s_Dispatcher; /**< @brief (Bullet Physics) Collision dispatcher instance.   */
		inline static std::shared_ptr<btBroadphaseInterface>                  s_Broadphase; /**< @brief (Bullet Physics) Broadphase instance.             */
		inline static std::shared_ptr<btSequentialImpulseConstraintSolver>        s_Solver; /**< @brief (Bullet Physics) Solver instance.                 */
		inline static std::shared_ptr<btDiscreteDynamicsWorld>             s_DynamicsWorld; /**< @brief (Bullet Physics) Dynamics world instance.         */
		
 		/** @brief Value of gravity. */
 		inline static glm::vec3 s_Gravity { 0.0, -9.82, 0.0 };
		
		/** @brief Time in seconds since the physics engine was last updated. */
		inline static tick_t s_LastTick { 0.0 };
		
		/** @brief Initialise the physics engine.*/
		static void Init();
		
		/** @brief Update the physics simulation by one step. */
		static void Tick(const tick_t& _step);
		
		/**
		 * @brief Set the value of gravity within the simulation.
		 * @param[in] _value The new value of gravity as a glm::vec3 object.
		 */
		static void Gravity(const glm::vec3& _value) noexcept;
		
		/**
		 * @brief Get the value of gravity within the simulation.
		 * @return The value of gravity as a glm::vec3 object.
		 */
		static const glm::vec3& Gravity() noexcept;
		
		/**
		 * @brief Render debugging information associated with the state of the physics engine.
		 *
		 * This function allows you to render debugging information such as wireframes, bounding boxes,
		 * contact points and normals for the physics engine. It uses the Bullet Physics library's
		 * btIDebugDraw interface to draw the debug information.
		 *
		 * @param[in] _debugMode (optional) The debug drawing mode. It specifies which types of debug shapes and information
		 *                       should be rendered. Defaults to drawing wireframes, AABBs, contact points, and normals.
		 *
		 * @see btIDebugDraw
		 * @see https://pybullet.org/Bullet/BulletFull/classbtIDebugDraw.html
		 */
		static void Debug(const int& _debugMode = (unsigned)btIDebugDraw::DBG_DrawWireframe     |
				                                  (unsigned)btIDebugDraw::DBG_DrawAabb          |
												  (unsigned)btIDebugDraw::DBG_DrawContactPoints |
												  (unsigned)btIDebugDraw::DBG_DrawNormals       );
		
		/** @brief Finalise the physics simulation. */
		static void Dispose();
		
	};
	
} // LouiEriksson::Engine::Physics

#endif //FINALYEARPROJECT_PHYSICS_H