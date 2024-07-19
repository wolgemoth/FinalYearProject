#ifndef FINALYEARPROJECT_PHYSICS_HPP
#define FINALYEARPROJECT_PHYSICS_HPP

#include "../core/Debug.hpp"
#include "../core/Time.hpp"
#include "../core/Types.hpp"

#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>

#include <LinearMath/btIDebugDraw.h>
#include <LinearMath/btScalar.h>
#include <LinearMath/btVector3.h>

#include <exception>
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
			
			explicit Debugger(const std::weak_ptr<btDynamicsWorld>& _dynamicsWorld) :
				m_DynamicsWorld(_dynamicsWorld)
			{
				try {
					
					if (const auto dw = m_DynamicsWorld.lock()) {
						dw->setDebugDrawer(this);
					}
				}
				catch (const std::exception& e) {
					Debug::Log(e);
				}
			}
			 
			/** @inheritdoc */
			~Debugger() override {
				
				try {
					
					if (const auto dw = m_DynamicsWorld.lock()) {
						dw->setDebugDrawer(nullptr);
					}
				}
				catch (const std::exception& e) {
					Debug::Log(e, Critical);
				}
			}
			
			/** @inheritdoc */
			[[deprecated("Not implemented")]]
			void drawLine(const btVector3 &_from, const btVector3 &_to, const btVector3 &_color) override {
				Debug::Break();
			}
			
			/** @inheritdoc */
			[[deprecated("Not implemented")]]
			void drawContactPoint(const btVector3 &_PointOnB, const btVector3 &_normalOnB, btScalar _distance, int _lifeTime, const btVector3 &_color) override {
				Debug::Break();
			}
			
			/** @inheritdoc */
			[[deprecated("Not implemented")]]
			void reportErrorWarning(const char *_warningString) override {
				Debug::Break();
			}
			
			/** @inheritdoc */
			[[deprecated("Not implemented")]]
			void draw3dText(const btVector3 &_location, const char *_textString) override {
				Debug::Break();
			}
			
			/** @inheritdoc */
			[[deprecated("Not implemented")]]
			void setDebugMode(int _debugMode) override {
				Debug::Break();
			}
			
			/** @inheritdoc */
			[[deprecated("Not implemented")]]
			[[nodiscard]] int getDebugMode() const override {
				
				Debug::Break();
				return 0;
			}
			
		};
		
		/**< @brief Instance of class used to visualise the state of the physics engine for debugging purposes. */
		inline static std::shared_ptr<Debugger> s_Debugger;
		
		inline static std::shared_ptr<btDefaultCollisionConfiguration>     s_Configuration; /**< @brief (Bullet Physics) Configuration of the simulation. */
		inline static std::shared_ptr<btCollisionDispatcher>                  s_Dispatcher; /**< @brief (Bullet Physics) Collision dispatcher instance.   */
		inline static std::shared_ptr<btBroadphaseInterface>                  s_Broadphase; /**< @brief (Bullet Physics) Broadphase instance.             */
		inline static std::shared_ptr<btSequentialImpulseConstraintSolver>        s_Solver; /**< @brief (Bullet Physics) Solver instance.                 */
		inline static std::shared_ptr<btDiscreteDynamicsWorld>             s_DynamicsWorld; /**< @brief (Bullet Physics) Dynamics world instance.         */
		
 		/** @brief Value of gravity. */
 		inline static vec3 s_Gravity { 0.0, -9.82, 0.0 };
		
		/** @brief Time in seconds since the physics engine was last updated. */
		inline static btScalar s_LastTick { 0.0 };
		
		/** @brief Initialise the physics engine.*/
		static void Init() {
		
			Debug::Log("Initialising Bullet Physics Engine...", Info);
			
			try {
				
				// Initialise the configuration of the physics system.
				Debug::Log("\tInitialising Default Configuration... ", Info, true);
				try {
					s_Configuration = std::make_shared<btDefaultCollisionConfiguration>();
					Debug::Log("Done.", Info);
				}
				catch (const std::exception& e) {
					Debug::Log("Failed.", Error);
					throw e;
				}
				
				// Initialise the collision dispatcher.
				Debug::Log("\tInitialising Dispatcher... ", Info, true);
				try {
					s_Dispatcher = std::make_shared<btCollisionDispatcher>(s_Configuration.get());
					Debug::Log("Done.", Info);
				}
				catch (const std::exception& e) {
					Debug::Log("Failed.", Error);
					throw e;
				}
				
				// Initialise the broadphase.
				Debug::Log("\tInitialising Broadphase... ", Info, true);
				try {
					s_Broadphase = std::make_shared<btDbvtBroadphase>();
					Debug::Log("Done.", Info);
				}
				catch (const std::exception& e) {
					Debug::Log("Failed.", Error);
					throw e;
				}
				
				// Initialise the solver.
				Debug::Log("\tInitialising Solver... ", Info, true);
				try {
					s_Solver = std::make_shared<btSequentialImpulseConstraintSolver>();
					Debug::Log("Done.", Info);
				}
				catch (const std::exception& e) {
					Debug::Log("Failed.", Error);
					throw e;
				}
				
				// Initialise the dynamics world.
				Debug::Log("\tInitialising Dynamics World... ", Info, true);
				try {
					s_DynamicsWorld = std::make_shared<btDiscreteDynamicsWorld>(s_Dispatcher.get(), s_Broadphase.get(), s_Solver.get(), s_Configuration.get());
					Debug::Log("Done.", Info);
				}
				catch (const std::exception& e) {
					Debug::Log("Failed.", Error);
					throw e;
				}
				
	//			// Create and initialise an instance of the visual debugging class.
	//			Debug::Log("\tInitialising Debug Drawer... ", Info, true);
	//			try {
	//				s_Debugger.reset(new Debugger(s_DynamicsWorld));
	//				Debug::Log("Done.", Info);
	//			}
	//			catch (const std::exception& e) {
	//				Debug::Log("Failed.", Error);
	//				throw e;
	//			}
			}
			catch (const std::exception& e) {
				Debug::Log("Failed.", Error);
				Debug::Log(e);
			}
		}
		
		/** @brief Update the physics simulation by one step. */
		static void Tick(const btScalar& _step) {
			
			const btScalar epsilon = 0.0001;
			
			if (_step > epsilon) {
				s_DynamicsWorld->stepSimulation(_step, 1, std::max(Time::FixedDeltaTime<btScalar>(), std::numeric_limits<btScalar>::min()));
			
				s_LastTick = 0.0;
			}
		}
		
		/**
		 * @brief Set the value of gravity within the simulation.
		 * @param[in] _value The new value of gravity as a vec3 object.
		 */
		static void Gravity(const vec3& _value) noexcept {
			s_Gravity = _value;
		}
		
		/**
		 * @brief Get the value of gravity within the simulation.
		 * @return The value of gravity as a vec3 object.
		 */
		static constexpr const vec3& Gravity() noexcept {
			return s_Gravity;
		}
		
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
		[[deprecated("Not implemented")]]
		static void Debug(const int& _debugMode = static_cast<signed>(
				static_cast<unsigned>(btIDebugDraw::DBG_DrawWireframe    ) |
				static_cast<unsigned>(btIDebugDraw::DBG_DrawAabb         ) |
				static_cast<unsigned>(btIDebugDraw::DBG_DrawContactPoints) |
				static_cast<unsigned>(btIDebugDraw::DBG_DrawNormals      ))
		) {
			
			try {
				
				if (s_Debugger != nullptr) {
					s_Debugger->setDebugMode(_debugMode);
					
					s_DynamicsWorld->debugDrawWorld();
				}
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
		}
		
		/** @brief Finalise the physics simulation. */
		static void Dispose() noexcept {
			
			     s_Debugger.reset();
			s_Configuration.reset();
			   s_Dispatcher.reset();
			   s_Broadphase.reset();
			       s_Solver.reset();
			s_DynamicsWorld.reset();
		}
		
	};
	
} // LouiEriksson::Engine::Physics

#endif //FINALYEARPROJECT_PHYSICS_HPP