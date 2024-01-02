#ifndef FINALYEARPROJECT_PHYSICS_H
#define FINALYEARPROJECT_PHYSICS_H

namespace LouiEriksson {
	
	class Physics {
	
		friend class Application;
		
		inline static std::shared_ptr<btDefaultCollisionConfiguration>     s_Configuration = nullptr;
		inline static std::shared_ptr<btCollisionDispatcher>               s_Dispatcher    = nullptr;
		inline static std::shared_ptr<btBroadphaseInterface>               s_Broadphase    = nullptr;
		inline static std::shared_ptr<btSequentialImpulseConstraintSolver> s_Solver        = nullptr;
		inline static std::shared_ptr<btDiscreteDynamicsWorld>             s_DynamicsWorld = nullptr;
		
	private:
		
		inline static glm::vec3 s_Gravity = glm::vec3(0.0f, -9.82f, 0.0f);
		
		static void Init();
		
		static void Tick();
		
		static void Dispose();
		
		static void Gravity(const glm::vec3& _value);
		
		static const glm::vec3& Gravity();
		
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_PHYSICS_H