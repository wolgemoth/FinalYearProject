#ifndef FINALYEARPROJECT_PLANETARIUM_H
#define FINALYEARPROJECT_PLANETARIUM_H

#include "../include/engine_core.h"
#include "../include/engine_spatial.h"

using namespace LouiEriksson::Engine;

namespace LouiEriksson::Game::Scripts {
	
	/// <summary>
	/// Planetarium.
	/// </summary>
	class Planetarium final : public Script {
	
	protected:
	
		/// <summary> Transform of the FlyCam. </summary>
		std::weak_ptr<Transform> m_Transform;
		
		/// <summary>
		/// String-indexed Hashmap holding strong pointers to planets as represented in the VSOP87 model.
		/// </summary>
		Hashmap<std::string, std::shared_ptr<ECS::GameObject>> m_Planets;
		
		/// <inheritdoc/>
		void Begin() override;
	
		/// <inheritdoc/>
		void Tick() override;
		
		static auto UNIX2JD(const double& _val);
		
		static auto JD2UNIX(const double& _val);
		
		static auto ET2JD(const double& _val);
		
		static auto JD2ET(const double& _val);
		
		static auto TT(const double& _tai);
		
		static auto Invert(const glm::vec3& _vec);
		
		void UpdatePlanets();
		
	public:
	
		explicit Planetarium(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		~Planetarium() override;
	
		[[nodiscard]] const std::type_index TypeID() const noexcept override { return typeid(Planetarium); };
	};
	
} // LouiEriksson::Game::Scripts

#endif //FINALYEARPROJECT_PLANETARIUM_H