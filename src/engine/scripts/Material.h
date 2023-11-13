#pragma once

#ifndef FINALYEARPROJECT_MATERIAL_H
#define FINALYEARPROJECT_MATERIAL_H

#include "Shader.h"
#include "Texture.h"

namespace LouiEriksson {
	
	class Material {
	
	private:
	
		// Shader the Material points to.
		std::shared_ptr<Shader> m_Shader;
	
		Texture m_Albedo;
		Texture m_Roughness;
		Texture m_Metallic;
		Texture m_Normals;
		Texture m_Height;
		Texture m_Detail;
		Texture m_AO;
		Texture m_Emission;
		
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
		
		[[nodiscard]] const Texture& GetAlbedo()    const;
		[[nodiscard]] const Texture& GetRoughness() const;
		[[nodiscard]] const Texture& GetMetallic()  const;
		[[nodiscard]] const Texture& GetNormals()   const;
		[[nodiscard]] const Texture& GetHeight()    const;
		[[nodiscard]] const Texture& GetDetail()    const;
		[[nodiscard]] const Texture& GetAO()        const;
		[[nodiscard]] const Texture& GetEmission()  const;
	};
}

#endif //FINALYEARPROJECT_MATERIAL_H