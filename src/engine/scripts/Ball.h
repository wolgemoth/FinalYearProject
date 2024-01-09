#ifndef FINALYEARPROJECT_BALL_H
#define FINALYEARPROJECT_BALL_H

#include "Scene.h"

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	/// <summary>
	/// Ball object with Rigidbody physics.
	/// </summary>
	class Ball final : public Script {
	
		friend class Scene;
	
	private:
		
		/// <summary> Mesh of the Ball. </summary>
		inline static std::shared_ptr<Mesh> s_Mesh;
		
		/// <summary> Material of the Ball. </summary>
		inline static std::weak_ptr<Material> s_Material;
	
		glm::vec3 m_StartingPosition;
		
	protected:
	
		/// <inheritdoc/>
		void Begin() override;
	
		/// <inheritdoc/>
		void Tick() override;
	
		/// <inheritdoc/>
		void FixedTick() override;
		
		/// <inheritdoc/>
		void OnCollision(const Collision& _collision) override;
		
	public:
	
		explicit Ball(const std::shared_ptr<GameObject>& _parent);
		~Ball() override;
	
	};
}

#endif //FINALYEARPROJECT_BALL_H