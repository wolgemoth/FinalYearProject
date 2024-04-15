#ifndef GAME_PLANE_H
#define GAME_PLANE_H

#include "../include/engine_core.h"

using namespace LouiEriksson::Engine;

namespace LouiEriksson::Game::Scripts {
	
	/**
	 * @class Plane
	 * @brief A Plane with Collision.
	 */
	class Plane final : public Script {
	
	protected:
	
		/** @brief Material of the plane. */
		inline static std::weak_ptr<Graphics::Material> m_Material;
	
		/** @inheritdoc */
		void Begin() override;
	
		/** @inheritdoc */
		void Tick() override;
	
	public:
	
		explicit Plane(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		
		/** @inheritdoc */
		~Plane() override;
	
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Plane); };
	};
	
} // LouiEriksson::Game::Scripts

#endif //GAME_PLANE_H