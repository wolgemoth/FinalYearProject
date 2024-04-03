#ifndef FINALYEARPROJECT_RESOURCES_H
#define FINALYEARPROJECT_RESOURCES_H

#include "../core/utils/Hashmap.h"
#include "../audio/AudioClip.h"
#include "../graphics/Material.h"
#include "../graphics/Mesh.h"
#include "../graphics/Shader.h"
#include "../graphics/Texture.h"

#include "File.h"

#include <exception>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <utility>

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
		
		enum Status : char {
			Unloaded,
			Loaded,
			Missing,
			Error
		};
	
		template <typename T>
		class Asset final {
		
			friend Resources;
			
		private:
			
			Status m_Status;
			
			std::filesystem::path m_Path;
			
			std::shared_ptr<T> m_Item;
			
			Asset() :
				m_Path(),
				m_Status(Unloaded),
				m_Item() {}
				
			Asset(std::filesystem::path  _path) :
				m_Path(std::move(_path)),
				m_Status(Unloaded),
				m_Item() {}
	
			void Load();
		};
		
		inline static Hashmap<std::string, Asset<   Audio::AudioClip>> m_Audio;
		inline static Hashmap<std::string, Asset<Graphics::Material >> m_Materials;
		inline static Hashmap<std::string, Asset<Graphics::Mesh     >> m_Meshes;
		inline static Hashmap<std::string, Asset<Graphics::Shader   >> m_Shaders;
		inline static Hashmap<std::string, Asset<Graphics::Texture  >> m_Textures;
		
		template<typename T>
		inline static Hashmap<std::string, Asset<T>>& GetBucket();
		
		static bool GetType(const std::string& _extension, std::type_index& _output);
		
		/// <summary>
		/// Index all of the assets the application has access to.
		/// </summary>
		static void IndexAssets();
		
		/// <summary>
		/// Index items which may other assets may be dependent on.
		/// </summary>
		static void IndexDependencies();
		
	public:
	
		static void Init();
		
		template<typename T>
		static void Preload(const std::string _name) {
			
			try {
				
				auto& item = GetBucket<T>().Return(_name);
				
				if (item.m_Status == Unloaded) {
					item.Load();
				}
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
		}
		
		template<typename T>
		inline static std::weak_ptr<T> Get(const std::string& _name, const bool& _fallback = true) noexcept {
			
			std::weak_ptr<T> result;
			
			try {
				
				auto& item = GetBucket<T>().Return(_name);
				
				switch (item.m_Status) {
					case Unloaded: {
						item.Load();
						result = item.m_Item;
						
						break;
					}
					case Loaded: {
						result = item.m_Item;
						
						break;
					}
					case Missing: {
						
						if (_fallback && _name != "missing") {
							result = Resources::Get<T>("missing");
						}
						
						break;
					}
					case Error: {
						
						if (_fallback && _name != "error") {
							result = Resources::Get<T>("error");
						}
						
						break;
					}
					default: {
						throw std::runtime_error("Not implemented!");
					}
				}
			}
			catch (const std::exception& e) {
				Debug::Log("Error accessing resource \"" + _name + "\". Reason : " + std::string(e.what()), LogType::Error);
			}
			
			return result;
		}
	};
	
	template<>
	inline Hashmap<std::string, Resources::Asset<Audio::AudioClip>>& Resources::GetBucket() {
		return m_Audio;
	}
	
	template<>
	inline Hashmap<std::string, Resources::Asset<Graphics::Material>>& Resources::GetBucket() {
		return m_Materials;
	}
	
	template<>
	inline Hashmap<std::string, Resources::Asset<Graphics::Mesh>>& Resources::GetBucket() {
		return m_Meshes;
	}
	
	template<>
	inline Hashmap<std::string, Resources::Asset<Graphics::Shader>>& Resources::GetBucket() {
		return m_Shaders;
	}
	
	template<>
	inline Hashmap<std::string, Resources::Asset<Graphics::Texture>>& Resources::GetBucket() {
		return m_Textures;
	}
	
	template<>
	inline void Resources::Asset<Audio::AudioClip>::Load() {

		try {
			
			if (exists(m_Path)) {
				
				if (File::TryLoad(m_Path, m_Item)) {
					m_Status = Loaded;
				}
				else {
					throw std::runtime_error("Error loading resource!");
				}
			}
			else {
				m_Status = Missing;
			}
		}
		catch (const std::exception& e) {
			Debug::Log(e);
			
			m_Status = Error;
		}
	}

	template<>
	inline void Resources::Asset<Graphics::Mesh>::Load() {

		try {
			
			if (exists(m_Path)) {
				
				if (File::TryLoad(m_Path, m_Item)) {
					m_Status = Loaded;
				}
				else {
					throw std::runtime_error("Error loading resource!");
				}
			}
			else {
				m_Status = Missing;
			}
		}
		catch (const std::exception& e) {
			Debug::Log(e);
			
			m_Status = Error;
		}
	}

	template<>
	inline void Resources::Asset<Graphics::Material>::Load() {

		try {
			
			if (exists(m_Path)) {
				
				if (File::TryLoad(m_Path, m_Item)) {
					m_Status = Loaded;
				}
				else {
					throw std::runtime_error("Error loading resource!");
				}
			}
			else {
				m_Status = Missing;
			}
		}
		catch (const std::exception& e) {
			Debug::Log(e);
			
			m_Status = Error;
		}
	}

	template<>
	inline void Resources::Asset<Graphics::Texture>::Load() {

		try {
			
			if (exists(m_Path)) {
				
				GLenum format = GL_SRGB;
				
				if (m_Path.has_extension()) {
				
					     if (m_Path.extension() == ".hdr") { format = GL_RGBA32F; }
					else if (m_Path.extension() == ".exr") { format = GL_RGBA32F; }
					else if (m_Path.has_parent_path()) {
						
						if (m_Path.parent_path().string().find("linear") != 0u) {
							format = GL_RGBA32F;
						}
					}
				}
				
				if (File::TryLoad(m_Path, m_Item, { format, true })) {
					m_Status = Loaded;
				}
				else {
					throw std::runtime_error("Error loading resource!");
				}
			}
			else {
				m_Status = Missing;
			}
		}
		catch (const std::exception& e) {
			Debug::Log(e);
			
			m_Status = Error;
		}
	}
	
	template<>
	inline void Resources::Asset<Graphics::Shader>::Load() {

		try {
			
			if (exists(m_Path)) {
				
				if (File::TryLoad(m_Path, m_Item)) {
					m_Status = Loaded;
				}
				else {
					throw std::runtime_error("Error loading resource!");
				}
			}
			else {
				m_Status = Missing;
			}
		}
		catch (const std::exception& e) {
			Debug::Log(e);
			
			m_Status = Error;
		}
	}
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_RESOURCES_H