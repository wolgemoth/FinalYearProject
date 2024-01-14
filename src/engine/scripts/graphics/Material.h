#ifndef FINALYEARPROJECT_MATERIAL_H
#define FINALYEARPROJECT_MATERIAL_H

#include <memory>

// @Assessor: This class was submitted for GACP. Please don't mark it for 3DGP.

namespace LouiEriksson {
	
	class File;
	
} // LouiEriksson

namespace LouiEriksson::Graphics {
	
	class Shader;
	class Texture;
	
} // LouiEriksson::Graphics

namespace LouiEriksson::Graphics {
	
	class Material {
	
		friend LouiEriksson::File;
		
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
		std::weak_ptr<Shader> GetShader() const noexcept;
		
		[[nodiscard]] std::weak_ptr<Texture> GetAlbedo()       const noexcept;
		[[nodiscard]] std::weak_ptr<Texture> GetRoughness()    const noexcept;
		[[nodiscard]] std::weak_ptr<Texture> GetMetallic()     const noexcept;
		[[nodiscard]] std::weak_ptr<Texture> GetNormals()      const noexcept;
		[[nodiscard]] std::weak_ptr<Texture> GetDisplacement() const noexcept;
		[[nodiscard]] std::weak_ptr<Texture> GetAO()           const noexcept;
		[[nodiscard]] std::weak_ptr<Texture> GetEmission()     const noexcept;
		
	};
	
} // LouiEriksson::Graphics

#endif //FINALYEARPROJECT_MATERIAL_H