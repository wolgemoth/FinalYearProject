#include "Material.h"

#include "../core/Resources.h"

#include "Shader.h"
#include "Texture.h"

#include <memory>

namespace LouiEriksson::Engine::Graphics {
	
	Material::Material() :
			m_Shader              (Resources::Get<Shader> ("pbr"   )),
			m_Albedo_Texture      (Resources::Get<Texture>("white" )),
			m_AO_Texture          (Resources::Get<Texture>("white" )),
			m_Displacement_Texture(Resources::Get<Texture>("white" )),
			m_Emission_Texture    (Resources::Get<Texture>("white" )),
			m_Metallic_Texture    (Resources::Get<Texture>("black" )),
			m_Normal_Texture      (Resources::Get<Texture>("normal")),
			m_Roughness_Texture   (Resources::Get<Texture>("black" )),
			m_Albedo_Color        (1.0),
			m_Emission_Color      (0.0),
			m_AO                  (1.0),
			m_Displacement        (0.3),
			m_Normal              (1.0),
			m_Roughness           (1.0) {}
	
	Material::~Material() = default;
	
	const std::weak_ptr<Shader>& Material::GetShader() const noexcept { return m_Shader; }
	
	const std::weak_ptr<Texture>& Material::GetAlbedoTexture()       const noexcept { return m_Albedo_Texture;       }
	const std::weak_ptr<Texture>& Material::GetAOTexture()           const noexcept { return m_AO_Texture;           }
	const std::weak_ptr<Texture>& Material::GetDisplacementTexture() const noexcept { return m_Displacement_Texture; }
	const std::weak_ptr<Texture>& Material::GetEmissionTexture()     const noexcept { return m_Emission_Texture;     }
	const std::weak_ptr<Texture>& Material::GetMetallicTexture()     const noexcept { return m_Metallic_Texture;     }
	const std::weak_ptr<Texture>& Material::GetNormalTexture()       const noexcept { return m_Normal_Texture;       }
	const std::weak_ptr<Texture>& Material::GetRoughnessTexture()    const noexcept { return m_Roughness_Texture;    }
	
	const glm::vec4& Material::GetAlbedoColor()   const noexcept { return m_Albedo_Color;   }
	const glm::vec3& Material::GetEmissionColor() const noexcept { return m_Emission_Color; }
	
	const float& Material::GetAOAmount()           const noexcept { return m_AO;           }
	const float& Material::GetDisplacementAmount() const noexcept { return m_Displacement; }
	const float& Material::GetNormalAmount()       const noexcept { return m_Normal;       }
	const float& Material::GetRoughnessAmount()    const noexcept { return m_Roughness;    }
	
} // LouiEriksson::Engine::Graphics