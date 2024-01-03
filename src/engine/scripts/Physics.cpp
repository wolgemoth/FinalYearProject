#include "stdafx.h"

#include "Physics.h"

#include "Time.h"

namespace LouiEriksson {
	
	void Physics::Init() {
	
		std::cout << "Initialising Bullet Physics Engine... \n\t";
		
		try {
			
			std::cout << "Initialising Default Configuration... ";
				s_Configuration.reset(new btDefaultCollisionConfiguration());
			std::cout << "Done.\n\t";
			
			std::cout << "Initialising Dispatcher... ";
				s_Dispatcher.reset(new btCollisionDispatcher(s_Configuration.get()));
			std::cout << "Done.\n\t";
			
			std::cout << "Initialising Broadphase... ";
				s_Broadphase.reset(new btDbvtBroadphase());
			std::cout << "Done.\n\t";
			
			std::cout << "Initialising Solver... ";
				s_Solver.reset(new btSequentialImpulseConstraintSolver());
			std::cout << "Done.\n\t";
			
			std::cout << "Initialising Dynamics World... ";
				s_DynamicsWorld.reset(new btDiscreteDynamicsWorld(s_Dispatcher.get(), s_Broadphase.get(), s_Solver.get(), s_Configuration.get()));
			std::cout << "Done.\n";
		}
		catch (const std::exception& e) {
			std::cout << "Failed.\n" << e.what() << '\n';
		}
	}
	
	void Physics::Gravity(const glm::vec3& _value) {
		s_Gravity = _value;
	}
	
	const glm::vec3& Physics::Gravity() {
		return s_Gravity;
	}
	
	void Physics::Dispose() {
	
	}
	
	void Physics::Tick() {
		s_DynamicsWorld->stepSimulation(Time::FixedDeltaTime());
	}
	
} // LouiEriksson