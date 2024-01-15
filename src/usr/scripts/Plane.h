#ifndef GAME_PLANE_H
#define GAME_PLANE_H

#include "../include/engine_core.h"

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson::Game {
	
	/// <summary>
	/// A Plane with Collision.
	/// </summary>
	class Plane final : public Script {
	
	protected:
	
		/// <summary> Material of the Plane. </summary>
		inline static std::weak_ptr<Graphics::Material> m_Material;
	
		/// <inheritdoc/>
		void Begin() override;
	
		/// <inheritdoc/>
		void Tick() override;
	
	public:
	
		explicit Plane(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		~Plane() override;
	
		std::type_index TypeID() const noexcept override { return typeid(Plane); };
	};
	
} // LouiEriksson::Game

#endif //GAME_PLANE_H