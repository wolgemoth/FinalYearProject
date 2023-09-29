#include "stdafx.h"

#include "Material.h"

Material::Material(const std::shared_ptr<Shader>& _shader) {

	m_Texture_ID = -1;

	m_Shader = _shader;
}

Material::~Material() {

}

std::shared_ptr<Material> Material::Create(const std::shared_ptr<Shader>& _shader) {

	// Return new instance wrapped in a shared pointer with custom deleter (as the destructor is private).
	return {new Material(_shader), [](Material* _ptr) { delete _ptr; }};
}


std::shared_ptr<Shader> Material::GetShader() {
	return m_Shader;
}

void Material::Texture_ID(GLuint _textureID) {
	m_Texture_ID = _textureID;
}

GLuint Material::Texture_ID() {
	return m_Texture_ID;
}
