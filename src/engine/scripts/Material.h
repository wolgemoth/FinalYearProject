#pragma once

#ifndef FINALYEARPROJECT_MATERIAL_H
#define FINALYEARPROJECT_MATERIAL_H

#include "Shader.h"
#include "Texture.h"

namespace LouiEriksson {
	
	class Material {
	
	private:
	
		// Shader the Material points to.
		std::weak_ptr<Shader> m_Shader;
	
		std::weak_ptr<Texture> m_Albedo;
		std::weak_ptr<Texture> m_Roughness;
		std::weak_ptr<Texture> m_Metallic;
		std::weak_ptr<Texture> m_Normals;
		std::weak_ptr<Texture> m_Height;
		std::weak_ptr<Texture> m_Detail;
		std::weak_ptr<Texture> m_AO;
		std::weak_ptr<Texture> m_Emission;
		
		explicit Material(const std::weak_ptr<Shader>& _shader);
		~Material();
		
	public:
		
		GLuint m_ProjectionMatrixID;
		GLuint      m_ModelMatrixID;
		GLuint       m_ViewMatrixID;
		
		/// <summary> Factory function which creates a new Material from a Shader. </summary>
		[[nodiscard]] static std::shared_ptr<Material> Create(const std::weak_ptr<Shader>& _shader);
		
		/// <summary> Get the currently assigned Shader. </summary>
		std::weak_ptr<Shader> GetShader();
		
		[[nodiscard]] const std::weak_ptr<Texture> GetAlbedo()    const;
		[[nodiscard]] const std::weak_ptr<Texture> GetRoughness() const;
		[[nodiscard]] const std::weak_ptr<Texture> GetMetallic()  const;
		[[nodiscard]] const std::weak_ptr<Texture> GetNormals()   const;
		[[nodiscard]] const std::weak_ptr<Texture> GetHeight()    const;
		[[nodiscard]] const std::weak_ptr<Texture> GetDetail()    const;
		[[nodiscard]] const std::weak_ptr<Texture> GetAO()        const;
		[[nodiscard]] const std::weak_ptr<Texture> GetEmission()  const;
	};
}

#endif //FINALYEARPROJECT_MATERIAL_H