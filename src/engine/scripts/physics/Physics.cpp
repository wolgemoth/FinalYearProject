#include "Physics.h"

#include "../core/Time.h"
#include "../core/Debug.h"

#include <LinearMath/btScalar.h>
#include <LinearMath/btVector3.h>

#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>

#include <glm/common.hpp>
#include <glm/ext/vector_float3.hpp>

#include <exception>
#include <memory>
#include <stdexcept>

namespace LouiEriksson::Engine::Physics {
	
	Physics::Debugger::Debugger(const std::weak_ptr<btDynamicsWorld>& _dynamicsWorld) :
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
	
	Physics::Debugger::~Debugger() {
		
		try {
			
			if (const auto dw = m_DynamicsWorld.lock()) {
				dw->setDebugDrawer(nullptr);
			}
		}
		catch (const std::exception& e) {
			Debug::Log(e);
		}
	}
	
	void Physics::Debugger::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
		Debug::Log("Not implemented", LogType::Error);
	}
	
	void Physics::Debugger::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {
		Debug::Log("Not implemented", LogType::Error);
	}
	
	void Physics::Debugger::reportErrorWarning(const char* warningString) {
		Debug::Log("Not implemented", LogType::Error);
	}
	
	void Physics::Debugger::draw3dText(const btVector3& location, const char* textString) {
		Debug::Log("Not implemented", LogType::Error);
	}
	
	void Physics::Debugger::setDebugMode(int debugMode) {
		Debug::Log("Not implemented", LogType::Error);
	}
	
	int Physics::Debugger::getDebugMode() const {
		Debug::Log("Not implemented", LogType::Error);
		
		return -1;
	}
	
	void Physics::Init() {
	
		Debug::Log("Initialising Bullet Physics Engine...", LogType::Info);
		
		try {
			
			// Initialise the configuration of the physics system.
			Debug::Log("\tInitialising Default Configuration... ", LogType::Info, true);
			try {
				s_Configuration.reset(new btDefaultCollisionConfiguration());
				Debug::Log("Done.", LogType::Info);
			}
			catch (const std::exception& e) {
				Debug::Log("Failed.", LogType::Error);
				throw e;
			}
			
			// Initialise the collision dispatcher.
			Debug::Log("\tInitialising Dispatcher... ", LogType::Info, true);
			try {
				s_Dispatcher.reset(new btCollisionDispatcher(s_Configuration.get()));
				Debug::Log("Done.", LogType::Info);
			}
			catch (const std::exception& e) {
				Debug::Log("Failed.", LogType::Error);
				throw e;
			}
			
			// Initialise the broadphase.
			Debug::Log("\tInitialising Broadphase... ", LogType::Info, true);
			try {
				s_Broadphase.reset(new btDbvtBroadphase());
				Debug::Log("Done.", LogType::Info);
			}
			catch (const std::exception& e) {
				Debug::Log("Failed.", LogType::Error);
				throw e;
			}
			
			// Initialise the solver.
			Debug::Log("\tInitialising Solver... ", LogType::Info, true);
			try {
				s_Solver.reset(new btSequentialImpulseConstraintSolver());
				Debug::Log("Done.", LogType::Info);
			}
			catch (const std::exception& e) {
				Debug::Log("Failed.", LogType::Error);
				throw e;
			}
			
			// Initialise the dynamics world.
			Debug::Log("\tInitialising Dynamics World... ", LogType::Info, true);
			try {
				s_DynamicsWorld.reset(new btDiscreteDynamicsWorld(s_Dispatcher.get(), s_Broadphase.get(), s_Solver.get(), s_Configuration.get()));
				Debug::Log("Done.", LogType::Info);
			}
			catch (const std::exception& e) {
				Debug::Log("Failed.", LogType::Error);
				throw e;
			}
			
//			// Create and initialise an instance of the visual debugging class.
//			Debug::Log("\tInitialising Debug Drawer... ", LogType::Info, true);
//			try {
//				s_Debugger.reset(new Debugger(s_DynamicsWorld));
//				Debug::Log("Done.", LogType::Info);
//			}
//			catch (const std::exception& e) {
//				Debug::Log("Failed.", LogType::Error);
//				throw e;
//			}
		}
		catch (const std::exception& e) {
			Debug::Log("Failed.", LogType::Error);
			Debug::Log(e);
		}
	}
	
	void Physics::Tick(const float& _step) {
		
		s_DynamicsWorld->stepSimulation(_step, 1, glm::max(Time::FixedDeltaTime(), __FLT_EPSILON__));
		
		if (_step != 0.0f) {
			s_LastTick = 0.0f;
		}
	}
	
	void Physics::Gravity(const glm::vec3& _value) noexcept {
		s_Gravity = _value;
	}
	const glm::vec3& Physics::Gravity() noexcept {
		return s_Gravity;
	}
	
	void Physics::Debug(const int& _debugMode) {
		
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
	
	void Physics::Dispose() {
	
		try {
		
		}
		catch (const std::exception& e) {
			Debug::Log(e, LogType::Critical);
		}
		
	}
	
} // LouiEriksson::Engine::Physics