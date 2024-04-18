#ifndef FINALYEARPROJECT_STARS_H
#define FINALYEARPROJECT_STARS_H

#include "../../include/engine_core.h"
#include "../../include/engine_spatial.h"

using namespace LouiEriksson::Engine;
using namespace LouiEriksson::Engine::Graphics;
using namespace LouiEriksson::Engine::Spatial;

namespace LouiEriksson::Game::Scripts::Spatial {
	
	/**
	 * @class Stars
	 * @brief 3D stellar field centered around Sol.
	 */
	class Stars final : public Script {
	
	protected:
	
		/** @inheritdoc */
		void Begin() override;
	
		/** @inheritdoc */
		void Tick() override;
	
		std::shared_ptr<Mesh> LoadStars(const std::vector<std::filesystem::path>& _athyg_paths, const double& _threshold_magnitude);

	public:
	
		explicit Stars(const std::weak_ptr<ECS::GameObject>& _parent);
		
		/** @inheritdoc */
		~Stars() override;
		
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Stars); };
	};
	
} // LouiEriksson::Game::Scripts::Spatial

#endif //FINALYEARPROJECT_STARS_H