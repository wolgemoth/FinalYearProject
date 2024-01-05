#ifndef FINALYEARPROJECT_PHYSICS_H
#define FINALYEARPROJECT_PHYSICS_H

namespace LouiEriksson {
	
	class Physics {
	
		friend class Application;
		friend class Rigidbody;
		
	private:
		
		/// <summary>
		/// Implementation of the abstract btIDebugDraw class.
		/// </summary>
		class Debugger : public btIDebugDraw {
		
		private:
			
			/// <summary> Weak pointer to the btDynamicsWorld linked to this debugger. </summary>
			std::weak_ptr<btDynamicsWorld> m_DynamicsWorld;
			
		public:
			
			 explicit Debugger(const std::weak_ptr<btDynamicsWorld>& _dynamicsWorld);
			~Debugger() override;
			
			/// <inheritdoc/>
			void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;
			
			/// <inheritdoc/>
			void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) override;
			
			/// <inheritdoc/>
			void reportErrorWarning(const char *warningString) override;
			
			/// <inheritdoc/>
			void draw3dText(const btVector3 &location, const char *textString) override;
			
			/// <inheritdoc/>
			void setDebugMode(int debugMode) override;
			
			/// <inheritdoc/>
			[[nodiscard]] int getDebugMode() const override;
		};
		
		/// <summary> Instance of class used to visualise the state of the physics engine for debugging purposes. </summary>
		inline static std::shared_ptr<Debugger> s_Debugger = nullptr;
		
		/// <summary> (Bullet Physics) Configuration of the simulation. </summary>
		inline static std::shared_ptr<btDefaultCollisionConfiguration> s_Configuration = nullptr;
		
		/// <summary> (Bullet Physics) Collision dispatcher instance. </summary>
		inline static std::shared_ptr<btCollisionDispatcher> s_Dispatcher = nullptr;
		
		/// <summary> (Bullet Physics) Broadphase instance. </summary>
		inline static std::shared_ptr<btBroadphaseInterface> s_Broadphase = nullptr;
		
		/// <summary> (Bullet Physics) Solver instance. </summary>
		inline static std::shared_ptr<btSequentialImpulseConstraintSolver> s_Solver = nullptr;
		
		/// <summary> (Bullet Physics) Dynamics world instance. </summary>
		inline static std::shared_ptr<btDiscreteDynamicsWorld> s_DynamicsWorld = nullptr;
		
		/// <summary> Value of gravity. </summary>
		inline static glm::vec3 s_Gravity = glm::vec3(0.0f, -9.82f, 0.0f);
		
		inline static float s_LastTick = 0.0f;
		
		/// <summary> Initialise the physics engine. </summary>
		static void Init();
		
		/// <summary> Update the physics simulation by one step. </summary>
		static void Tick(const float& _step);
		
		/// <summary> Set the value of gravity within the simulation. </summary>
		static void Gravity(const glm::vec3& _value);
		
		/// <summary> Get the value of gravity within the simulation. </summary>
		static const glm::vec3& Gravity();
		
		/// <summary>
		/// Render debugging information associated with the state of the physics engine.
		/// See also: <see href="https://pybullet.org/Bullet/BulletFull/classbtIDebugDraw.html">btIDebugDraw</see>
		/// </summary>
		static void Debug(const int& _debugMode = btIDebugDraw::DBG_DrawWireframe     |
				                                  btIDebugDraw::DBG_DrawAabb          |
												  btIDebugDraw::DBG_DrawContactPoints |
												  btIDebugDraw::DBG_DrawNormals       );
		
		/// <summary> Finalise the physics simulation. </summary>
		static void Dispose();
		
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_PHYSICS_H