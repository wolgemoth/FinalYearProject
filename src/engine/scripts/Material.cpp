#include "stdafx.h"

#include "Material.h"

namespace LouiEriksson {
	
	Material::Material(const std::shared_ptr<Shader>& _shader) {
	
		m_ProjectionMatrixID = _shader->AttributeID("u_Projection");
		     m_ModelMatrixID = _shader->AttributeID("u_Model");
		      m_ViewMatrixID = _shader->AttributeID("u_View");
		
		m_Shader = _shader;
	}
	
	Material::~Material() = default;
	
	std::shared_ptr<Material> Material::Create(const std::shared_ptr<Shader>& _shader) {
	
		// Return a new instance wrapped in a shared pointer with a custom deleter (as it has a private destructor).
		return { new Material(_shader), [](Material* _ptr) { delete _ptr; } };
	}
	
	std::shared_ptr<Shader> Material::GetShader() {
		return m_Shader;
	}
	
	const Texture& Material::GetAlbedo() const {
		return m_Albedo;
	}
	
	const Texture& Material::GetRoughness() const {
		return m_Roughness;
	}
	
	const Texture& Material::GetMetallic() const {
		return m_Metallic;
	}
	
	const Texture& Material::GetNormals() const {
		return m_Normals;
	}
	
	const Texture& Material::GetHeight() const {
		return m_Height;
	}
	
	const Texture& Material::GetDetail() const {
		return m_Detail;
	}
	
	const Texture& Material::GetAO() const {
		return m_AO;
	}
	
	const Texture& Material::GetEmission() const {
		return m_Emission;
	}
}