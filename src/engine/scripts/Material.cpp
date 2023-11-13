#include "stdafx.h"

#include "Material.h"

#include "Resources.h"

namespace LouiEriksson {
	
	Material::Material(const std::weak_ptr<Shader>& _shader) {
	
		m_Shader = _shader;
		
		m_ProjectionMatrixID = m_Shader.lock()->AttributeID("u_Projection");
		     m_ModelMatrixID = m_Shader.lock()->AttributeID("u_Model");
		      m_ViewMatrixID = m_Shader.lock()->AttributeID("u_View");
			  
		m_Albedo    = Resources::GetTexture("black");
		m_Roughness = Resources::GetTexture("black");
		m_Metallic  = Resources::GetTexture("black");
		m_Normals   = Resources::GetTexture("black");
		m_Height    = Resources::GetTexture("black");
		m_Detail    = Resources::GetTexture("black");
		m_AO        = Resources::GetTexture("black");
		m_Emission  = Resources::GetTexture("black");
	}
	
	Material::~Material() = default;
	
	std::shared_ptr<Material> Material::Create(const std::weak_ptr<Shader>& _shader) {
	
		// Return a new instance wrapped in a shared pointer with a custom deleter (as it has a private destructor).
		return { new Material(_shader), [](Material* _ptr) { delete _ptr; } };
	}
	
	std::weak_ptr<Shader> Material::GetShader() {
		return m_Shader;
	}
	
	const std::weak_ptr<Texture> Material::GetAlbedo() const {
		return m_Albedo;
	}
	
	const std::weak_ptr<Texture> Material::GetRoughness() const {
		return m_Roughness;
	}
	
	const std::weak_ptr<Texture> Material::GetMetallic() const {
		return m_Metallic;
	}
	
	const std::weak_ptr<Texture> Material::GetNormals() const {
		return m_Normals;
	}
	
	const std::weak_ptr<Texture> Material::GetHeight() const {
		return m_Height;
	}
	
	const std::weak_ptr<Texture> Material::GetDetail() const {
		return m_Detail;
	}
	
	const std::weak_ptr<Texture> Material::GetAO() const {
		return m_AO;
	}
	
	const std::weak_ptr<Texture> Material::GetEmission() const {
		return m_Emission;
	}
}