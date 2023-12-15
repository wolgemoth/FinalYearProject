#ifndef FINALYEARPROJECT_MATERIAL_H
#define FINALYEARPROJECT_MATERIAL_H

#include "Shader.h"
#include "Texture.h"

class File;

// @Assessor: This class was submitted for GACP. Please don't mark it for 3DGP.

namespace LouiEriksson {
	
	class Material {
	
		friend class File;
		
	private:
	
		// Shader the Material points to.
		std::weak_ptr<Shader> m_Shader;
	
		std::weak_ptr<Texture> m_Albedo;
		std::weak_ptr<Texture> m_Roughness;
		std::weak_ptr<Texture> m_Metallic;
		std::weak_ptr<Texture> m_Normals;
		std::weak_ptr<Texture> m_Displacement;
		std::weak_ptr<Texture> m_AO;
		std::weak_ptr<Texture> m_Emission;
		
		 Material();
		 
	public:
		
		~Material();
		
		/// <summary> Get the currently assigned Shader. </summary>
		std::weak_ptr<Shader> GetShader();
		
		[[nodiscard]] std::weak_ptr<Texture> GetAlbedo()       const;
		[[nodiscard]] std::weak_ptr<Texture> GetRoughness()    const;
		[[nodiscard]] std::weak_ptr<Texture> GetMetallic()     const;
		[[nodiscard]] std::weak_ptr<Texture> GetNormals()      const;
		[[nodiscard]] std::weak_ptr<Texture> GetDisplacement() const;
		[[nodiscard]] std::weak_ptr<Texture> GetAO()           const;
		[[nodiscard]] std::weak_ptr<Texture> GetEmission()     const;
	};
}

#endif //FINALYEARPROJECT_MATERIAL_H