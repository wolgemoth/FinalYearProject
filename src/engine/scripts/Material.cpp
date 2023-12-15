#include "stdafx.h"

#include "Material.h"
#include "Resources.h"

// @Assessor: This class was submitted for GACP. Please don't mark it for 3DGP.

namespace LouiEriksson {
	
	Material::Material() {
	
		m_Shader = Resources::GetShader("pbr");
		
		m_Shader.lock()->AttributeID("u_Projection");
		     m_Shader.lock()->AttributeID("u_Model");
		      m_Shader.lock()->AttributeID("u_View");
		
		m_Albedo       = Resources::GetTexture("white");
		m_Roughness    = Resources::GetTexture("black");
		m_Metallic     = Resources::GetTexture("black");
		m_Normals      = Resources::GetTexture("normal");
		m_Displacement = Resources::GetTexture("black");
		m_AO           = Resources::GetTexture("white");
		m_Emission     = Resources::GetTexture("white");
	}
	
	Material::~Material() = default;
	
	std::weak_ptr<Shader> Material::GetShader() {
		return m_Shader;
	}
	
	std::weak_ptr<Texture> Material::GetAlbedo() const {
		return m_Albedo;
	}
	
	std::weak_ptr<Texture> Material::GetRoughness() const {
		return m_Roughness;
	}
	
	std::weak_ptr<Texture> Material::GetMetallic() const {
		return m_Metallic;
	}
	
	std::weak_ptr<Texture> Material::GetNormals() const {
		return m_Normals;
	}
	
	std::weak_ptr<Texture> Material::GetDisplacement() const {
		return m_Displacement;
	}
	
	std::weak_ptr<Texture> Material::GetAO() const {
		return m_AO;
	}
	
	std::weak_ptr<Texture> Material::GetEmission() const {
		return m_Emission;
	}
}