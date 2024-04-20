#ifndef FINALYEARPROJECT_MATERIAL_H
#define FINALYEARPROJECT_MATERIAL_H

#include <memory>

namespace LouiEriksson::Engine {
	
	class Resources;
}

namespace LouiEriksson::Engine::Graphics {
	
	class Shader;
	class Texture;
	
} // LouiEriksson::Engine::Graphics

namespace LouiEriksson::Engine::Graphics {
	
	class Material final {
	
		friend LouiEriksson::Engine::Resources;
		
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
		
		GLfloat m_AO,
			    m_Displacement,
			    m_Normal,
		        m_Roughness;
		
		 Material();
		 
	public:
		
		/**
		 * \brief Get the currently assigned Shader.
		 *
		 * \return A weak pointer to the currently assigned Shader.
		 */
		[[nodiscard]] constexpr const std::weak_ptr<Shader>& GetShader() const noexcept { return m_Shader; }
		
		[[nodiscard]] constexpr const std::weak_ptr<Texture>& GetAlbedoTexture()       const noexcept { return m_Albedo_Texture;       }
		[[nodiscard]] constexpr const std::weak_ptr<Texture>& GetAOTexture()           const noexcept { return m_AO_Texture;           }
		[[nodiscard]] constexpr const std::weak_ptr<Texture>& GetDisplacementTexture() const noexcept { return m_Displacement_Texture; }
		[[nodiscard]] constexpr const std::weak_ptr<Texture>& GetEmissionTexture()     const noexcept { return m_Emission_Texture;     }
		[[nodiscard]] constexpr const std::weak_ptr<Texture>& GetMetallicTexture()     const noexcept { return m_Metallic_Texture;     }
		[[nodiscard]] constexpr const std::weak_ptr<Texture>& GetNormalTexture()       const noexcept { return m_Normal_Texture;       }
		[[nodiscard]] constexpr const std::weak_ptr<Texture>& GetRoughnessTexture()    const noexcept { return m_Roughness_Texture;    }
		
		[[nodiscard]] constexpr const glm::vec4& GetAlbedoColor()   const noexcept { return m_Albedo_Color;   }
		[[nodiscard]] constexpr const glm::vec3& GetEmissionColor() const noexcept { return m_Emission_Color; }
		
		[[nodiscard]] constexpr const GLfloat& GetAOAmount()           const noexcept { return m_AO;           }
		[[nodiscard]] constexpr const GLfloat& GetDisplacementAmount() const noexcept { return m_Displacement; }
		[[nodiscard]] constexpr const GLfloat& GetNormalAmount()       const noexcept { return m_Normal;       }
		[[nodiscard]] constexpr const GLfloat& GetRoughnessAmount()    const noexcept { return m_Roughness;    }
		
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_MATERIAL_H