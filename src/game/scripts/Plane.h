#ifndef GAME_PLANE_H
#define GAME_PLANE_H

#include "../include/engine_core.h"

using namespace LouiEriksson::Engine;

namespace LouiEriksson::Game::Scripts {
	
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
	
		[[nodiscard]] const std::type_index TypeID() const noexcept override { return typeid(Plane); };
	};
	
} // LouiEriksson::Game::Scripts

#endif //GAME_PLANE_H