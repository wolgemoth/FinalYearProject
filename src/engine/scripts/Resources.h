#pragma once

#ifndef FINALYEARPROJECT_RESOURCES_H
#define FINALYEARPROJECT_RESOURCES_H

#include "File.h"

namespace LouiEriksson {
	
	class Resources {
	
	private:
		
		static inline std::filesystem::path         m_MeshesDirectory = "models/";
		static inline std::filesystem::path      m_MaterialsDirectory = "materials/";
		static inline std::filesystem::path   m_TexturesSRGBDirectory = "textures/srgb";
		static inline std::filesystem::path m_TexturesLinearDirectory = "textures/linear";
		static inline std::filesystem::path  m_ShaderIncludeDirectory = "shaders/include";
		static inline std::filesystem::path m_ShaderProgramsDirectory = "shaders/programs";
		
		static inline Hashmap<std::string, std::shared_ptr<Mesh    >> m_Meshes;
		static inline Hashmap<std::string, std::shared_ptr<Material>> m_Materials;
		static inline Hashmap<std::string, std::shared_ptr<Texture >> m_Textures;
		static inline Hashmap<std::string, std::shared_ptr<Shader  >> m_Shaders;
		
		/// <summary>
		/// Preload meshes and add them to the cache.
		/// </summary>
		static void PreloadMeshes();
		
		/// <summary>
		/// Preload materials and add them to the cache.
		/// </summary>
		static void PreloadMaterials();
		
		/// <summary>
		/// Preload textures and add them to the cache.
		/// </summary>
		static void PreloadTextures();
		
		/// <summary>
		/// Preload shaders and add them to the cache.
		/// </summary>
		static void PreloadShaders();
		
	public:
	
		static void Preload();
	
		static bool TryGetMesh(const std::string& _name, std::shared_ptr<Mesh>& _output);
		
		static bool TryGetMaterial(const std::string& _name, std::shared_ptr<Material>& _output);
		
		static bool TryGetTexture(const std::string& _name, std::shared_ptr<Texture>& _output);
		
		static bool TryGetShader(const std::string& _name, std::shared_ptr<Shader>& _output);
		
		static std::weak_ptr<Mesh> GetMesh(const std::string& _name);
		
		static std::weak_ptr<Material> GetMaterial(const std::string& _name);
		
		static std::weak_ptr<Texture> GetTexture(const std::string& _name);
		
		static std::weak_ptr<Shader> GetShader(const std::string& _name);
		
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_RESOURCES_H