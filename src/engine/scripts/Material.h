#pragma once

#ifndef FINALYEARPROJECT_MATERIAL_H
#define FINALYEARPROJECT_MATERIAL_H

#include "Shader.h"

namespace LouiEriksson {
	
	class Material {
	
	private:
	
		// Shader the Material points to.
		std::shared_ptr<Shader> m_Shader;
	
		// Texture ID of the Material.
		GLuint m_Texture_ID;
	
		explicit Material(const std::shared_ptr<Shader>& _shader);
		~Material();
	
	public:
		
		GLuint m_ProjectionMatrixID;
		GLuint      m_ModelMatrixID;
		GLuint       m_ViewMatrixID;
		
		/// <summary> Factory function which creates a new Material from a Shader. </summary>
		[[nodiscard]] static std::shared_ptr<Material> Create(const std::shared_ptr<Shader>& _shader);
		
		/// <summary> Get the currently assigned Shader. </summary>
		std::shared_ptr<Shader> GetShader();
		
		/// <summary> Set the current Texture. </summary>
		void Texture_ID(GLuint _textureID);
		
		/// <summary> Get the current Texture. </summary>
		[[nodiscard]] const GLuint& Texture_ID() const;
		
		explicit operator GLuint() const { return this->Texture_ID(); }
	
	};
}

#endif //FINALYEARPROJECT_MATERIAL_H