#ifndef FINALYEARPROJECT_RESOURCES_H
#define FINALYEARPROJECT_RESOURCES_H

#include "File.h"

// @Assessor: This class was submitted for GEP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	class Resources {
	
	private:
		
		inline static std::filesystem::path          m_AudioDirectory = "audio/";
		inline static std::filesystem::path         m_MeshesDirectory = "models/";
		inline static std::filesystem::path      m_MaterialsDirectory = "materials/";
		inline static std::filesystem::path   m_TexturesSRGBDirectory = "textures/srgb";
		inline static std::filesystem::path m_TexturesLinearDirectory = "textures/linear";
		inline static std::filesystem::path  m_ShaderIncludeDirectory = "shaders/include";
		inline static std::filesystem::path m_ShaderProgramsDirectory = "shaders/programs";
		
		inline static Hashmap<std::string, std::shared_ptr<AudioClip>> m_Audio;
		inline static Hashmap<std::string, std::shared_ptr<Mesh     >> m_Meshes;
		inline static Hashmap<std::string, std::shared_ptr<Material >> m_Materials;
		inline static Hashmap<std::string, std::shared_ptr<Texture  >> m_Textures;
		inline static Hashmap<std::string, std::shared_ptr<Shader   >> m_Shaders;
		
		/// <summary>
		/// Preload meshes and add them to the cache.
		/// </summary>
		static void PreloadAudio();
		
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
	
		static bool TryGetAudio(const std::string& _name, std::shared_ptr<AudioClip>& _output);
		
		static bool TryGetMesh(const std::string& _name, std::shared_ptr<Mesh>& _output);
		
		static bool TryGetMaterial(const std::string& _name, std::shared_ptr<Material>& _output);
		
		static bool TryGetTexture(const std::string& _name, std::shared_ptr<Texture>& _output);
		
		static bool TryGetShader(const std::string& _name, std::shared_ptr<Shader>& _output);
		
		static std::weak_ptr<AudioClip> GetAudio(const std::string& _name);
		
		static std::weak_ptr<Mesh> GetMesh(const std::string& _name);
		
		static std::weak_ptr<Material> GetMaterial(const std::string& _name);
		
		static std::weak_ptr<Texture> GetTexture(const std::string& _name);
		
		static std::weak_ptr<Shader> GetShader(const std::string& _name);
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_RESOURCES_H