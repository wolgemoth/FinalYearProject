#ifndef FINALYEARPROJECT_MATERIAL_H
#define FINALYEARPROJECT_MATERIAL_H

#include <memory>

namespace LouiEriksson::Engine {
	
	class File;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Graphics {
	
	class Shader;
	class Texture;
	
} // LouiEriksson::Engine::Graphics

namespace LouiEriksson::Engine::Graphics {
	
	class Material {
	
		friend LouiEriksson::Engine::File;
		
	private:
	
		// Shader the Material points to.
		std::weak_ptr<Shader> m_Shader;
	
		std::weak_ptr<Texture> m_Albedo_Texture,
		                       m_AO_Texture,
		                       m_Displacement_Texture,
		                       m_Emission_Texture,
		                       m_Metallic_Texture,
		                       m_Normal_Texture,
		                       m_Roughness_Texture;
		
		glm::vec4 m_Albedo_Color;
		
		glm::vec3 m_Emission_Color;
		
		float m_AO,
			  m_Displacement,
			  m_Normal,
		      m_Roughness;
		
		 Material();
		 
	public:
		
		~Material();
		
		/**
		 * \brief Get the currently assigned Shader.
		 *
		 * \return A weak pointer to the currently assigned Shader.
		 */
		[[nodiscard]] const std::weak_ptr<Shader>& GetShader() const noexcept;
		
		[[nodiscard]] const std::weak_ptr<Texture>& GetAlbedoTexture()       const noexcept;
		[[nodiscard]] const std::weak_ptr<Texture>& GetAOTexture()           const noexcept;
		[[nodiscard]] const std::weak_ptr<Texture>& GetDisplacementTexture() const noexcept;
		[[nodiscard]] const std::weak_ptr<Texture>& GetEmissionTexture()     const noexcept;
		[[nodiscard]] const std::weak_ptr<Texture>& GetMetallicTexture()     const noexcept;
		[[nodiscard]] const std::weak_ptr<Texture>& GetNormalTexture()      const noexcept;
		[[nodiscard]] const std::weak_ptr<Texture>& GetRoughnessTexture()    const noexcept;
		
		[[nodiscard]] const glm::vec4& GetAlbedoColor()   const noexcept;
		[[nodiscard]] const glm::vec3& GetEmissionColor() const noexcept;
		
		[[nodiscard]] const float& GetAOAmount()           const noexcept;
		[[nodiscard]] const float& GetDisplacementAmount() const noexcept;
		[[nodiscard]] const float& GetNormalAmount()       const noexcept;
		[[nodiscard]] const float& GetRoughnessAmount()    const noexcept;
		
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_MATERIAL_H