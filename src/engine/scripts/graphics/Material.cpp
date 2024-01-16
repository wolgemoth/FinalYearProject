#include "Material.h"

#include "../core/Resources.h"

#include "Shader.h"
#include "Texture.h"

#include <memory>

namespace LouiEriksson::Engine::Graphics {
	
	Material::Material() :
			m_Shader      (Resources::GetShader ("pbr"  )),
			m_Albedo      (Resources::GetTexture("white")),
			m_Roughness   (Resources::GetTexture("black")),
			m_Metallic    (Resources::GetTexture("black")),
			m_Normals     (Resources::GetTexture("normal")),
			m_Displacement(Resources::GetTexture("black")),
			m_AO          (Resources::GetTexture("white")),
			m_Emission    (Resources::GetTexture("white")) {}
	
	Material::~Material() = default;
	
	const std::weak_ptr<Shader>& Material::GetShader() const noexcept { return m_Shader; }
	
	const std::weak_ptr<Texture>& Material::GetAlbedo()       const noexcept { return m_Albedo;       }
	const std::weak_ptr<Texture>& Material::GetRoughness()    const noexcept { return m_Roughness;    }
	const std::weak_ptr<Texture>& Material::GetMetallic()     const noexcept { return m_Metallic;     }
	const std::weak_ptr<Texture>& Material::GetNormals()      const noexcept { return m_Normals;      }
	const std::weak_ptr<Texture>& Material::GetDisplacement() const noexcept { return m_Displacement; }
	const std::weak_ptr<Texture>& Material::GetAO()           const noexcept { return m_AO;           }
	const std::weak_ptr<Texture>& Material::GetEmission()     const noexcept { return m_Emission;     }
	
} // LouiEriksson::Engine::Graphics