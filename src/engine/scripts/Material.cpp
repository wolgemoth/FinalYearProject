#include "stdafx.h"

#include "Material.h"

namespace LouiEriksson {
	
	Material::Material(const std::shared_ptr<Shader>& _shader) {
	
		m_Texture_ID = -1;
	
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
	
	void Material::Texture_ID(GLuint _textureID) {
		m_Texture_ID = _textureID;
	}
	
	GLuint Material::Texture_ID() const {
		return m_Texture_ID;
	}
}