#include "Material.h"

#include "../core/Resources.h"

namespace LouiEriksson::Engine::Graphics {
	
	Material::Material() :
		m_Shader              (Resources::Get<Shader> ("pbr"   )),
		m_Albedo_Texture      (Resources::Get<Texture>("white" )),
		m_AO_Texture          (Resources::Get<Texture>("white" )),
		m_Displacement_Texture(Resources::Get<Texture>("black" )),
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
		
} // LouiEriksson::Engine::Graphics