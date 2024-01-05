#include "stdafx.h"

#include "Physics.h"

namespace LouiEriksson {
	
	Physics::Debugger::Debugger(const std::weak_ptr<btDynamicsWorld>& _dynamicsWorld) {
		
		try {
			
			m_DynamicsWorld = _dynamicsWorld;
			
			const auto dynamicsWorld = m_DynamicsWorld.lock();
			
			if (dynamicsWorld != nullptr) {
				dynamicsWorld->setDebugDrawer(this);
			}
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
		}
	}
	
	Physics::Debugger::~Debugger() {
		
		try {
			
			const auto dynamicsWorld = m_DynamicsWorld.lock();
			
			if (dynamicsWorld != nullptr) {
				dynamicsWorld->setDebugDrawer(nullptr);
			}
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
		}
	}
	
	void Physics::Debugger::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
		throw std::runtime_error("Not implemented!");
	}
	
	void Physics::Debugger::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {
		throw std::runtime_error("Not implemented!");
	}
	
	void Physics::Debugger::reportErrorWarning(const char* warningString) {
		throw std::runtime_error("Not implemented!");
	}
	
	void Physics::Debugger::draw3dText(const btVector3& location, const char* textString) {
		throw std::runtime_error("Not implemented!");
	}
	
	void Physics::Debugger::setDebugMode(int debugMode) {
		throw std::runtime_error("Not implemented!");
	}
	
	int Physics::Debugger::getDebugMode() const {
		throw std::runtime_error("Not implemented!");
	}
	
	void Physics::Init() {
	
		std::cout << "Initialising Bullet Physics Engine... \n\t";
		
		try {
			
			// Initialise the configuration of the physics system.
			std::cout << "Initialising Default Configuration... ";
				s_Configuration.reset(new btDefaultCollisionConfiguration());
			std::cout << "Done.\n\t";
			
			// Initialise the collision dispatcher.
			std::cout << "Initialising Dispatcher... ";
				s_Dispatcher.reset(new btCollisionDispatcher(s_Configuration.get()));
			std::cout << "Done.\n\t";
			
			// Initialise the broadphase.
			std::cout << "Initialising Broadphase... ";
				s_Broadphase.reset(new btDbvtBroadphase());
			std::cout << "Done.\n\t";
			
			// Initialise the solver.
			std::cout << "Initialising Solver... ";
				s_Solver.reset(new btSequentialImpulseConstraintSolver());
			std::cout << "Done.\n\t";
			
			// Initialise the dynamics world.
			std::cout << "Initialising Dynamics World... ";
				s_DynamicsWorld.reset(new btDiscreteDynamicsWorld(s_Dispatcher.get(), s_Broadphase.get(), s_Solver.get(), s_Configuration.get()));
			std::cout << "Done.\n";
			
			// Create and initialise an instance of the visual debugging class.
			std::cout << "Initialising Debug Drawer... ";
				//s_Debugger.reset(new Debugger(s_DynamicsWorld));
			std::cout << "Done.\n";
		}
		catch (const std::exception& e) {
			std::cout << "Failed.\n" << e.what() << '\n';
		}
	}
	
	void Physics::Tick(const float& _step) {
		s_DynamicsWorld->stepSimulation(_step);
		
		s_LastTick = 0.0f;
	}
	
	void Physics::Gravity(const glm::vec3& _value) {
		s_Gravity = _value;
	}
	
	const glm::vec3& Physics::Gravity() {
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
			std::cout << e.what() << '\n';
		}
	}
	
	void Physics::Dispose() {
	
	}
	
} // LouiEriksson