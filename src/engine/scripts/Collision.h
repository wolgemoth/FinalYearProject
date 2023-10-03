#pragma once

#ifndef FINALYEARPROJECT_COLLISION_H
#define FINALYEARPROJECT_COLLISION_H

namespace LouiEriksson {
	
	/// <summary>
	/// Contains information describing a collision between a Rigidbody and a Collider.
	/// </summary>
	struct Collision {
	
	private:
	
		/// <summary> Contact point of the Collision. </summary>
		glm::vec3 m_ContactPoint;
	
		/// <summary> Normal of the Collision. </summary>
		glm::vec3 m_Normal;
	
		/// <summary> Impulse of the Collision. </summary>
		glm::vec3 m_Impulse;
	
	public:
	
		 Collision();
		~Collision() = default;
	
		/// <summary> Set the contact point of the Collision. </summary>
		void ContactPoint(const glm::vec3& _contactPoint);
	
		/// <summary> Get the contact point of the Collision. </summary>
		const glm::vec3& ContactPoint();
	
		/// <summary> Set the normal of the Collision. </summary>
		void Normal(const glm::vec3& _normal);
		
		/// <summary> Get the normal of the Collision. </summary>
		const glm::vec3& Normal();
	
		/// <summary> Set the impulse of the Collision. </summary>
		void Impulse(const glm::vec3& _contactPoint);
		
		/// <summary> Get the impulse of the Collision. </summary>
		const glm::vec3& Impulse();
	};
}

#endif //FINALYEARPROJECT_COLLISION_H