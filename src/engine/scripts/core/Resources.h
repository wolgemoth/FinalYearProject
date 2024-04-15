#ifndef FINALYEARPROJECT_RESOURCES_H
#define FINALYEARPROJECT_RESOURCES_H

#include "../audio/AudioClip.h"
#include "../graphics/Material.h"
#include "../graphics/Mesh.h"
#include "../graphics/Shader.h"
#include "../graphics/Texture.h"
#include "utils/Hashmap.h"

#include "Debug.h"
#include "File.h"

#include <array>
#include <exception>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
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
			Unloaded, /**< @brief File is not yet loaded into memory. */
			  Loaded, /**< @brief File is currently loaded into memory. */
			 Missing, /**< @brief File not found. */
			   Error  /**< @brief Error loading file. */
		};
	
		template <typename T>
		class Asset final {
		
			friend Resources;
			
		private:
			
			std::filesystem::path m_Path;
			
			Status m_Status;
			
			std::shared_ptr<T> m_Item;
			
			Asset() :
				m_Path(),
				m_Status(Unloaded),
				m_Item() {}
				
			Asset(std::filesystem::path  _path) :
				m_Path(std::move(_path)),
				m_Status(Unloaded),
				m_Item() {}
			
			/**
			 * @brief Loads the asset.
			 *
			 * This function is responsible for loading the asset.
			 * It checks if the file exists and attempts to load the asset if it does.
			 * If the asset is successfully loaded, the status is set to "Loaded".
			 * If the file does not exist, the status is set to "Missing".
			 * If any error occurs during loading, the error message is logged and the status is set to "Error".
			 *
			 * @note This function is specific to the Asset class and is not intended to be called directly by the user.
			 */
			void Load();
		};
		
		inline static Hashmap<std::string, Asset<   Audio::AudioClip>> m_Audio;
		inline static Hashmap<std::string, Asset<Graphics::Material >> m_Materials;
		inline static Hashmap<std::string, Asset<Graphics::Mesh     >> m_Meshes;
		inline static Hashmap<std::string, Asset<Graphics::Shader   >> m_Shaders;
		inline static Hashmap<std::string, Asset<Graphics::Texture  >> m_Textures;
		inline static Hashmap<std::string, Asset<Graphics::Cubemap  >> m_Cubemaps;
		
		template<typename T>
		inline static Hashmap<std::string, Asset<T>>& GetBucket();
		
		static bool GetType(const std::string& _extension, std::type_index& _output);
		
		/**
		 * @brief Index all of the assets the application has access to.
		 *
		 * This function indexes all assets in the "assets/" directory and assigns them to the appropriate asset bucket based on their type.
		 * The assets are assigned to the asset buckets using the stem of their file name as the key.
		 *
		 * @note Assets with unsupported file extensions are logged as warnings.
		 * @see Asset
		 */
		static void IndexAssets();
		
		/**
		 * @brief Index items which other assets may be dependent on.
		 *
		 * @note This function should be called before loading any assets that may have dependencies.
		 */
		static void IndexDependencies();
		
	public:
	
		/**
		 * @brief Initialises the Resources system.
		 */
		static void Init();
		
		/**
		 * @brief Loads a specified asset if not already loaded.
		 *
		 * @tparam T The type of asset to load.
		 * @param[in] _name The key name of the asset to preload.
		 *
		 * @note This function uses the following external code: GetBucket<T>(), Asset<T>::Load(), Debug::Log(std::exception&)
		 *
		 * @see GetBucket()
		 * @see Asset<T>::Load()
		 * @see Debug::Log()
		 */
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
		
		/**
		 * @brief Retrieves the asset with the specified name.
		 *
		 * This function returns a std::weak_ptr to the asset with the specified name.
		 * If the asset is not loaded, it will be loaded and the status will be set to Loaded.
		 * If the asset is already loaded, the function simply returns the existing std::weak_ptr.
		 * If the asset is missing or in an error state, and fallback is set to true, the function will attempt to load the "missing" or "error" asset respectively.
		 * If the asset is missing or in an error state, and fallback is set to false, the function will return an empty std::weak_ptr.
		 *
		 * @tparam T The type of asset to retrieve.
		 * @param[in] _name The name of the asset to retrieve.
		 * @param[in] _fallback Whether to fallback to the "missing" or "error" asset if the requested asset is missing or in an error state.
		 * @return std::weak_ptr<T> A weak pointer to the requested asset.
		 *
		 * @note This function logs an error message if there is an exception while accessing the resource.
		 */
		template<typename T>
		inline static std::weak_ptr<T> Get(const std::string& _name, const bool& _fallback = true) noexcept {
			
			std::weak_ptr<T> result;
			
			try {
				
				auto& item = GetBucket<T>()[_name];
				
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
	inline Hashmap<std::string, Resources::Asset<Graphics::Cubemap>>& Resources::GetBucket() {
		return m_Cubemaps;
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
				Debug::Log("Invalid path \"" + m_Path.string() + "\"", LogType::Error);
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
				Debug::Log("Invalid path \"" + m_Path.string() + "\"", LogType::Error);
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
				Debug::Log("Invalid path \"" + m_Path.string() + "\"", LogType::Error);
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
				Debug::Log("Invalid path \"" + m_Path.string() + "\"", LogType::Error);
				m_Status = Missing;
			}
		}
		catch (const std::exception& e) {
			Debug::Log(e);
			
			m_Status = Error;
		}
	}
	
	template<>
	inline void Resources::Asset<Graphics::Cubemap>::Load() {

		try {
			
			if (exists(m_Path)) {
				
				if (is_directory(m_Path)) {
				
					std::array<std::filesystem::path, 6> faces;
					
					auto files = File::Directory::GetEntries(m_Path, File::Directory::EntryType::FILE);
					
					for (const auto& file : files) {
						
						const auto name = file.stem().string();
					    const auto suffix = name.substr(name.length() - 2);
					    
						if      (suffix == "px") { faces[0] = file; }
						else if (suffix == "nx") { faces[1] = file; }
						else if (suffix == "py") { faces[2] = file; }
						else if (suffix == "ny") { faces[3] = file; }
						else if (suffix == "pz") { faces[4] = file; }
						else if (suffix == "nz") { faces[5] = file; }
						else {
							Debug::Log("Unknown cubemap face \"" + suffix + "\"", LogType::Warning);
						}
					}
					
					if (File::TryLoad(faces, m_Item, { GL_RGB32F, true }, { GL_LINEAR, GL_LINEAR }, { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE })) {
						m_Status = Loaded;
					}
					else {
						throw std::runtime_error("Error loading resource!");
					}
				}
				else {
					throw std::runtime_error("Path is not directory!");
				}
			}
			else {
				Debug::Log("Invalid path \"" + m_Path.string() + "\"", LogType::Error);
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
	
	template<>
	inline std::weak_ptr<Graphics::Cubemap> Resources::Get(const std::string& _name, const bool& _fallback) noexcept {
		
		std::weak_ptr<Graphics::Cubemap> result;
		
		try {
			
			Asset<Graphics::Cubemap> item;
			if (!Resources::GetBucket<Graphics::Cubemap>().Get(_name, item)) {
				item = { _name };
			}
			
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
						result = Resources::Get<Graphics::Cubemap>("missing");
					}
					
					break;
				}
				case Error: {
					
					if (_fallback && _name != "error") {
						result = Resources::Get<Graphics::Cubemap>("error");
					}
					
					break;
				}
				default: {
					throw std::runtime_error("Not implemented!");
				}
			}
			
			Resources::GetBucket<Graphics::Cubemap>().Assign(_name, item);
		}
		catch (const std::exception& e) {
			Debug::Log("Error accessing resource \"" + _name + "\". Reason : " + std::string(e.what()), LogType::Error);
		}
		
		return result;
	}
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_RESOURCES_H