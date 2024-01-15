#ifndef FINALYEARPROJECT_PLANE_H
#define FINALYEARPROJECT_PLANE_H

#include "../../engine/scripts/core/Script.h"
#include "../../engine/scripts/graphics/Material.h"
#include "../../engine/scripts/graphics/Mesh.h"

#include "Component.h"

#include <memory>

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson::Game {
	
	/// <summary>
	/// A Plane with Collision.
	/// </summary>
	class Plane final : public Script {
	
	protected:
	
		/// <summary> Mesh of the Plane. </summary>
		inline static std::shared_ptr<Graphics::Mesh> m_Mesh { nullptr };
		
		/// <summary> Material of the Plane. </summary>
		inline static std::weak_ptr<Graphics::Material> m_Material;
	
		/// <inheritdoc/>
		void Begin() override;
	
		/// <inheritdoc/>
		void Tick() override;
	
	public:
	
		explicit Plane(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		~Plane() override;
	
	};
	
} // LouiEriksson::Game

#endif //FINALYEARPROJECT_PLANE_H