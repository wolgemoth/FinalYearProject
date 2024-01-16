#ifndef FINALYEARPROJECT_MATERIAL_H
#define FINALYEARPROJECT_MATERIAL_H

#include <memory>

namespace LouiEriksson::Engine {
	
	class File;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Graphics {
	
	class Shader;
	class Texture;
	
} // LouiEriksson::Engine::Graphics

namespace LouiEriksson::Engine::Graphics {
	
	class Material {
	
		friend LouiEriksson::Engine::File;
		
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
		[[nodiscard]] const std::weak_ptr<Shader>& GetShader() const noexcept;
		
		[[nodiscard]] const std::weak_ptr<Texture>& GetAlbedo()       const noexcept;
		[[nodiscard]] const std::weak_ptr<Texture>& GetRoughness()    const noexcept;
		[[nodiscard]] const std::weak_ptr<Texture>& GetMetallic()     const noexcept;
		[[nodiscard]] const std::weak_ptr<Texture>& GetNormals()      const noexcept;
		[[nodiscard]] const std::weak_ptr<Texture>& GetDisplacement() const noexcept;
		[[nodiscard]] const std::weak_ptr<Texture>& GetAO()           const noexcept;
		[[nodiscard]] const std::weak_ptr<Texture>& GetEmission()     const noexcept;
		
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_MATERIAL_H