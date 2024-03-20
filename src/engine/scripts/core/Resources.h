#ifndef FINALYEARPROJECT_RESOURCES_H
#define FINALYEARPROJECT_RESOURCES_H

#include "../core/utils/Hashmap.h"

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

namespace LouiEriksson::Engine::Audio {
	
	class AudioClip;
	
} // LouiEriksson::Engine::Audio

namespace LouiEriksson::Engine::Graphics {
	
	class Material;
	class Mesh;
	class Shader;
	class Texture;
	
} // LouiEriksson::Engine::Graphics

namespace LouiEriksson::Engine {
	
	class Resources {
	
	private:
		
		inline static std::filesystem::path          m_AudioDirectory { "audio/"                };
		inline static std::filesystem::path         m_MeshesDirectory { "models/"               };
		inline static std::filesystem::path      m_MaterialsDirectory { "materials/"            };
		inline static std::filesystem::path   m_TexturesSRGBDirectory { "textures/srgb"         };
		inline static std::filesystem::path m_TexturesLinearDirectory { "textures/linear"       };
		inline static std::filesystem::path  m_ShaderIncludeDirectory { "shaders/core/include"  };
		inline static std::filesystem::path m_ShaderProgramsDirectory { "shaders/core/programs" };
		
		inline static Hashmap<std::string, std::shared_ptr<   Audio::AudioClip>> m_Audio;
		inline static Hashmap<std::string, std::shared_ptr<Graphics::Mesh     >> m_Meshes;
		inline static Hashmap<std::string, std::shared_ptr<Graphics::Material >> m_Materials;
		inline static Hashmap<std::string, std::shared_ptr<Graphics::Texture  >> m_Textures;
		inline static Hashmap<std::string, std::shared_ptr<Graphics::Shader   >> m_Shaders;
		
		inline static Hashmap<std::type_index, Hashmap<std::string, std::string>> m_Failed;
		
		/// <summary>
		/// Add a key to the list of assets which failed loading.
		/// </summary>
		template<typename T>
		static void AddFailed(std::string _name, std::string _reason) {
			
			Hashmap<std::string, std::string> category;
			if (!m_Failed.Get(typeid(T), category)) {
				
				category = {};
				
				m_Failed.Assign(typeid(T), category);
			}
			
			if (!category.Add(_name, _reason)) {
				std::cerr << "Duplicate entry detected.\n";
			}
		}
		
		/// <summary>
		/// Checks for the existence of an asset which failed loading.
		/// </summary>
		template<typename T>
		static bool GetFailed(std::string _name, std::string& _reason) {
			
			bool result = false;
			
			Hashmap<std::string, std::string> category;
			if (m_Failed.Get(typeid(T), category)) {
				
				result = category.Get(_name, _reason);
			}
			
			return result;
		}
		
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
	
		static std::weak_ptr<Audio::AudioClip> GetAudio(const std::string& _name, const bool& _fallback = true) noexcept;
		
		static std::weak_ptr<Graphics::Mesh> GetMesh(const std::string& _name, const bool& _fallback = true) noexcept;
	 
		static std::weak_ptr<Graphics::Material> GetMaterial(const std::string& _name, const bool& _fallback = true) noexcept;
	 
		static std::weak_ptr<Graphics::Texture> GetTexture(const std::string& _name, const bool& _fallback = true) noexcept;
	 
		static std::weak_ptr<Graphics::Shader> GetShader(const std::string& _name, const bool& _fallback = true) noexcept;
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_RESOURCES_H