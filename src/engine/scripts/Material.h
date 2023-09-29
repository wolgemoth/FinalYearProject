#pragma once

#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "Shader.h"

class Material {
	
private:

	// Shader the Material points to.
	std::shared_ptr<Shader> m_Shader;

	// Texture ID of the Material.
	GLuint m_Texture_ID;

	Material(const std::shared_ptr<Shader>& _shader);
	~Material();

public:

	/// <summary> Factory function which creates a new Material from a Shader. </summary>
	[[nodiscard]] static std::shared_ptr<Material> Create(const std::shared_ptr<Shader>& _shader);

	/// <summary> Get the currently assigned Shader. </summary>
	std::shared_ptr<Shader> GetShader();
	
	/// <summary> Set the current Texture. </summary>
	void Texture_ID(GLuint _texture);

	/// <summary> Get the current Texture. </summary>
	GLuint Texture_ID();

};

#endif // !_MATERIAL_H
