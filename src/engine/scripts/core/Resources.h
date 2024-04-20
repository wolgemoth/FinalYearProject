#ifndef FINALYEARPROJECT_RESOURCES_H
#define FINALYEARPROJECT_RESOURCES_H

#include "../audio/AudioClip.h"
#include "../graphics/Material.h"
#include "../graphics/Mesh.h"
#include "../graphics/Shader.h"
#include "../graphics/Texture.h"
#include "../graphics/textures/Cubemap.h"
#include "File.h"
#include "utils/Hashmap.h"

#include "Debug.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h" // STB IMAGE
#endif

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GL/glew.h>
#include <glm/common.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <ios>
#include <limits>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <utility>
#include <vector>

namespace LouiEriksson::Engine {
	
	class Resources final {
	
	private:
		
		enum Status : unsigned char {
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
			
			Asset() noexcept :
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
		
		inline static const Hashmap<std::string, std::type_index> s_Types {
			{ ".wav",  typeid(   Audio::AudioClip) },
			{ ".obj",  typeid(Graphics::Mesh     ) },
			{ ".mtl",  typeid(Graphics::Material ) },
			{ ".jpg",  typeid(Graphics::Texture  ) },
			{ ".png",  typeid(Graphics::Texture  ) },
			{ ".tif",  typeid(Graphics::Texture  ) },
			{ ".hdr",  typeid(Graphics::Texture  ) },
			{ ".exr",  typeid(Graphics::Texture  ) },
			{ ".vert", typeid(Graphics::Shader   ) },
			{ ".geom", typeid(Graphics::Shader   ) },
			{ ".frag", typeid(Graphics::Shader   ) },
			{ ".glsl", typeid(Graphics::Shader   ) },
		};
		
		template<typename T>
		static constexpr Hashmap<std::string, Asset<T>>& GetBucket() {

			Hashmap<std::string, Asset<T>>* r;
			
			     if constexpr (std::is_same_v<T,    Audio::AudioClip>) { r = &m_Audio;     }
			else if constexpr (std::is_same_v<T, Graphics::Material >) { r = &m_Materials; }
			else if constexpr (std::is_same_v<T, Graphics::Mesh     >) { r = &m_Meshes;    }
			else if constexpr (std::is_same_v<T, Graphics::Shader   >) { r = &m_Shaders;   }
			else if constexpr (std::is_same_v<T, Graphics::Texture  >) { r = &m_Textures;  }
			else if constexpr (std::is_same_v<T, Graphics::Cubemap  >) { r = &m_Cubemaps;  }
			else {
				static_assert([]{ return false; }(), "Not implemented!");
			}
			
			return *r;
		}
		
		/**
		 * @brief Index all of the assets the application has access to.
		 *
		 * This function indexes all assets in the "assets/" directory and assigns them to the appropriate asset bucket based on their type.
		 * The assets are assigned to the asset buckets using the stem of their file name as the key.
		 *
		 * @note Assets with unsupported file extensions are logged as warnings.
		 * @see Asset
		 */
		static void IndexAssets() {
			
			for (const auto& item : File::Directory::GetEntriesRecursive("assets/", File::Directory::EntryType::FILE)) {
			
				if (item.has_extension()) {
				
					if (const auto type = s_Types.Get(item.extension())) {
						   
						     if (*type == typeid(   Audio::AudioClip)) {     m_Audio.Emplace(item.stem().string(), { item }); }
						else if (*type == typeid(Graphics::Material )) { m_Materials.Emplace(item.stem().string(), { item }); }
						else if (*type == typeid(Graphics::Mesh     )) {    m_Meshes.Emplace(item.stem().string(), { item }); }
						else if (*type == typeid(Graphics::Shader   )) {   m_Shaders.Emplace(item.stem().string(), { item }); }
						else if (*type == typeid(Graphics::Texture  )) {  m_Textures.Emplace(item.stem().string(), { item }); }
					}
					else {
						Debug::Log("Unable to determine the type of asset with extension " + item.extension().string(), LogType::Warning);
					}
				}
			}
		}
		
		/**
		 * @brief Index items which other assets may be dependent on.
		 *
		 * @note This function should be called before loading any assets that may have dependencies.
		 */
		inline static void IndexDependencies()  {
			
			/* INCLUDE SHADER DEPENDENCIES */
			{
				std::vector<std::filesystem::path> dependencies;
				
				for (const auto& item : File::Directory::GetEntriesRecursive("assets/", File::Directory::EntryType::FILE)) {
					
					if (strcmp(item.extension().string().c_str(), ".inc") == 0) {
						dependencies.emplace_back(item);
					}
				}
				
				for (const auto& dependency : dependencies) {
				
					Debug::Log("Loading Shader Dependency \"" + dependency.string() + "\"... ", LogType::Info, true);
					
					try {
						const auto name = "/" + dependency.string();
						const auto contents = (File::ReadAllText(dependency) << "\n").str();
						
						// https://www.opengl.org/registry/specs/ARB/shading_language_include.txt
						glNamedStringARB(
							GL_SHADER_INCLUDE_ARB,
							    static_cast<GLint>(name.length()),
							    name.c_str(),
							static_cast<GLint>(contents.length()),
							contents.c_str()
						);
						
						Debug::Log("Done.", LogType::Info);
					}
					catch (const std::exception& e) {
						Debug::Log("Failed.", LogType::Error);
						Debug::Log(e);
					}
				}
			}
		}
		
		static bool TryLoad(const std::filesystem::path& _path, std::shared_ptr<Audio::AudioClip>& _output) {
			
			bool result;
			
			Debug::Log("Loading AudioClip \"" + _path.string() + "\"...", LogType::Info, true);
			
			try {
				_output = std::make_shared<Audio::AudioClip>(_path);
				Debug::Log("Done.", LogType::Info);
				
				result = true;
			}
			catch (const std::exception& e) {
				Debug::Log("Failed.", LogType::Error);
				Debug::Log(e);
				
				result = false;
			}
			
			return result;
		}
		
		static bool TryLoad(const std::filesystem::path& _path, std::shared_ptr<Graphics::Texture>& _output,
			const Graphics::Texture::Parameters::Format&     _format,
			const Graphics::Texture::Parameters::FilterMode& _filterMode,
			const Graphics::Texture::Parameters::WrapMode&   _wrapMode
		) {
			
			bool result = false;
			
			Debug::Log("Loading Texture \"" + _path.string() + "\"... ", LogType::Info, true);
			
			try {
				
				_output.reset(
					new Graphics::Texture(-1, -1, 0, _format, _filterMode, _wrapMode)
				);
				
				glGenTextures(1, &_output->m_TextureID);
				
				if (_output->m_TextureID != GL_NONE) {
					
					int channels;
					GLenum texture_format;
					
					Graphics::Texture::GetFormatData(_format.PixelFormat(), texture_format, channels);
					
					void* data;
					GLenum data_format;
					
					if (strcmp(_path.extension().string().c_str(), ".hdr") == 0 ||
					    strcmp(_path.extension().string().c_str(), ".exr") == 0
					) {
						data_format = GL_FLOAT;
						data = stbi_loadf(
							_path.string().c_str(),
							&_output->m_Width,
							&_output->m_Height,
							nullptr,
							channels
						);
					}
					else {
						data_format = GL_UNSIGNED_BYTE;
						data = stbi_load(
							_path.string().c_str(),
							&_output->m_Width,
							&_output->m_Height,
							nullptr,
							channels
						);
					}
					
					if (data != nullptr) {
					
						Graphics::Texture::Bind(*_output);
						
						glTexImage2D(
							GL_TEXTURE_2D,
							0,
							static_cast<GLint>(_format.PixelFormat()),
							_output->m_Width,
							_output->m_Height,
							0,
							texture_format,
							   data_format,
							   data
						);
						
						stbi_image_free(data);
						
						if (_format.Mips()) {
							glGenerateMipmap(GL_TEXTURE_2D);
							
							const auto min = _output->FilterMode().Min();
							
							switch (min) {
								case GL_NEAREST: {
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
									break;
								}
								case GL_LINEAR:  {
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
									break;
								}
								case GL_NEAREST_MIPMAP_NEAREST:
								case GL_NEAREST_MIPMAP_LINEAR:
								case GL_LINEAR_MIPMAP_NEAREST:
								case GL_LINEAR_MIPMAP_LINEAR: {
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min));
									break;
								}
								default: {
									
									Debug::Log("Unknown (possibly unsupported) mipmap filtering value \"" + std::to_string(min) + "\". ", LogType::Warning, true);
									
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min));
									break;
								}
							}
						}
						else {
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(_output->FilterMode().Min()));
						}
						
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(_output->FilterMode().Mag()));
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
						
						// Get maximum possible anisotropy:
						GLfloat maxAnisotropy;
						glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
						
						// Set texture anisotropy:
						glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
						
						result = true;
						
						Debug::Log("Done.", LogType::Info);
					}
					else {
						throw std::runtime_error("Failed loading texture data!");
					}
				}
			}
			catch (const std::exception& e) {
				Debug::Log("Failed.", LogType::Error);
				Debug::Log(e);
			}
			
			return result;
		}
		
		static bool TryLoad(const std::filesystem::path& _path, std::shared_ptr<Graphics::Mesh>& _output) {
			
			bool result = false;
			
			Debug::Log("Loading Mesh \"" + _path.string() + "\"... ", LogType::Info, true);
			
			/*
			 * Implementation derived from Mesh.cpp and Mesh.h
			 * provided with OBJLoader_v3 project in GACP labs.
			 *
			 * I added tangent calculations to the code derived from an implementation by Learn OpenGL:
	         * de Vries, J. (n.d.). LearnOpenGL - Normal Mapping. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Normal-Mapping [Accessed 15 Dec. 2023].
			 */
			
			try {
				
				Assimp::Importer importer;
				const auto* const scene = importer.ReadFile(_path,
						aiProcess_Triangulate | aiProcess_ImproveCacheLocality |
						aiProcess_RemoveRedundantMaterials | aiProcess_OptimizeMeshes);
				
				assert(scene != nullptr &&
					"No scene!");
				
				assert((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) == 0u &&
					"Incomplete!");
				
				assert(scene->mRootNode != nullptr &&
					"No root node!");
				
				assert(scene->HasMeshes() &&
					"No meshes!");
				
				for (size_t i = 0; i < scene->mNumMeshes; i++) {
					
					const auto* const mesh = scene->mMeshes[i];
					
					assert(mesh->HasPositions() && "Mesh has no vertices!");
					assert(mesh->HasNormals()   && "Mesh has no normals!" );
					
					/* VERTEX DATA */
					
					// Get vertices, normals, and texture coordinates:
					std::vector<glm::vec3> vertices;
					std::vector<glm::vec3>  normals;
					std::vector<glm::vec2>      uvs;
					
					vertices.reserve(mesh->mNumVertices);
					 normals.reserve(mesh->mNumVertices);
					     uvs.reserve(mesh->mNumVertices);
					
					for (size_t j = 0; j < mesh->mNumVertices; ++j) {
						
						const auto vert = mesh->mVertices[j];
						const auto norm = mesh->mNormals [j];
						const auto   uv = mesh->mTextureCoords[i][j];
						
						vertices.emplace_back(vert.x, vert.y, vert.z);
						 normals.emplace_back(norm.x, norm.y, norm.z);
						     uvs.emplace_back(  uv.x,   uv.y        );
					}
					
					// Get or compute tangents:
					std::array<std::vector<glm::vec3>, 2> tangents;
					
					if (mesh->HasTangentsAndBitangents()) {
						
						tangents = {
							std::vector<glm::vec3>(),
							std::vector<glm::vec3>()
						};
						
						tangents[0].reserve(vertices.size());
						tangents[1].reserve(vertices.size());
						
						for (size_t j = 0; j < mesh->mNumVertices; ++j) {
							
							const auto  tan =   mesh->mTangents[j];
							const auto btan = mesh->mBitangents[j];
							
							tangents[0].emplace_back( tan.x,  tan.y,  tan.z);
							tangents[1].emplace_back(btan.x, btan.y, btan.z);
						}
					}
					else {
						tangents = Graphics::Mesh::GenerateTangents(vertices, uvs);
					}
					
					/* INDEX DATA */
					
					const auto* const faces = mesh->mFaces;
					
					// Determine if the mesh should use 8, 16, or 32-bit indices:
					if (mesh->mNumVertices > std::numeric_limits<GLushort>::max()) {
						
						const size_t limit32 = std::numeric_limits<GLuint>::max();
						Debug::Assert(
								mesh->mNumVertices <= limit32, "Vertex count exceeds the 32-bit limit and will be truncated. ",
								LogType::Warning, true
						);
						
						// 32-bit:
						std::vector<GLuint> indices;
						indices.reserve(mesh->mNumVertices);
						
						for (size_t j = 0; j < mesh->mNumFaces; ++j) {
							for (size_t k = 0; k < faces[j].mNumIndices; ++k) {
								indices.emplace_back(faces[j].mIndices[k]);
							}
						}
						
						_output = Graphics::Mesh::Create(vertices, indices, normals, uvs, tangents, GL_TRIANGLES);
					}
					else if (mesh->mNumVertices > std::numeric_limits<GLubyte>::max()) {
						
						// 16-bit:
						std::vector<GLushort> indices;
						indices.reserve(mesh->mNumVertices);
						
						for (size_t j = 0; j < mesh->mNumFaces; ++j) {
							for (size_t k = 0; k < faces[j].mNumIndices; ++k) {
								indices.emplace_back(faces[j].mIndices[k]);
							}
						}
						
						_output = Graphics::Mesh::Create(vertices, indices, normals, uvs, tangents, GL_TRIANGLES);
					}
					else {
						
						// 8-bit:
						std::vector<GLubyte> indices;
						indices.reserve(mesh->mNumVertices);
						
						for (size_t j = 0; j < mesh->mNumFaces; ++j) {
							for (size_t k = 0; k < faces[j].mNumIndices; ++k) {
								indices.emplace_back(faces[j].mIndices[k]);
							}
						}
						
						_output = Graphics::Mesh::Create(vertices, indices, normals, uvs, tangents, GL_TRIANGLES);
					}
					
					break;
				}
				
				Debug::Log("Done.", LogType::Info);
				
				result = true;
			}
			catch (const std::exception& e) {
				Debug::Log("Failed.", LogType::Error);
				Debug::Log(e);
			}
			
			return result;
		}
		
		static bool TryLoad(const std::filesystem::path& _path, std::shared_ptr<Graphics::Material>& _output) {
			
			bool result = false;
			
			Debug::Log("Loading Material \"" + _path.string() + "\"... ", LogType::Info, true);
			
			try {
				
				_output.reset(new Graphics::Material());
				
				std::fstream fs;
				fs.open(_path, std::ios::in);
			
				if (fs.is_open()) {
			
					std::string line;
			
					while (std::getline(fs, line)) {
						
						auto subStrings = Utils::Split(line, ' ');
						
						if (!subStrings.empty()) {
							
							const auto& key = subStrings.at(0);
							
							if (key == "Ka") {
								Debug::Log("Ambient color loading not implemented... ", LogType::Warning, true);
							}
							else if (key == "Kd") {
								
								auto r = Utils::Parse<float>(subStrings.at(1));
								auto g = Utils::Parse<float>(subStrings.at(2));
								auto b = Utils::Parse<float>(subStrings.at(3));
								
								_output->m_Albedo_Color = { r, g, b, 1.0 };
								
								try {
									_output->m_Albedo_Color.a = Utils::Parse<float>(subStrings.at(4));
								}
								catch (...) {}
							}
							else if (key == "d" || key == "Tr") {
								_output->m_Albedo_Color.a = static_cast<float>(1.0 - std::clamp(Utils::Parse<double>(subStrings.at(1)), 0.0, 1.0));
							}
							else if (key == "Ks") {
								Debug::Log("Specular color loading not implemented... ", LogType::Warning, true);
							}
							else if (key == "illum") {
								Debug::Log("Support for different lighting models is not implemented... ", LogType::Warning, true);
							}
							else if (key == "Ao") {
								_output->m_AO = Utils::Parse<float>(subStrings.at(1));
							}
							else if (key == "Ns") {
								_output->m_Roughness = static_cast<float>(1.0 - (std::atan(Utils::Parse<float>(subStrings.at(1))) / (M_PI / 2)));
							}
							else if (key == "Ke") {
								
								_output->m_Emission_Color = {
										Utils::Parse<float>(subStrings.at(1)),
										Utils::Parse<float>(subStrings.at(2)),
										Utils::Parse<float>(subStrings.at(3))
								};
							}
							else if (key == "Ni") {
								Debug::Log("Optical density loading not implemented... ", LogType::Warning, true);
							}
							else if (key == "map_Kd") {
								
								if (subStrings.size() >= 2) {
								
									const std::filesystem::path path(subStrings.at(1));
									
									if (const auto texture = Resources::Get<Graphics::Texture>(path.stem().string()).lock()) {
										_output->m_Albedo_Texture = texture;
									}
								}
							}
							else if (key == "map_Ks") {
								Debug::Log("Specular map loading not implemented... ", LogType::Warning, true);
							}
							else if (key == "map_Tr" || key == "map_d") {
								Debug::Log("Transparency map loading not implemented... ", LogType::Warning, true);
							}
							else if (key == "bump" || key == "map_bump" || key == "bm") {
								Debug::Log("Bump map loading not implemented... ", LogType::Warning, true);
							}
							else if (key == "disp") {
								
								if (subStrings.size() >= 2) {
								
									const std::filesystem::path path(subStrings.at(1));
									
									if (const auto texture = Resources::Get<Graphics::Texture>(path.stem().string()).lock()) {
										_output->m_Displacement_Texture = texture;
									}
								}
							}
							else if (key == "map_Pr") {
								
								if (subStrings.size() >= 2) {
								
									const std::filesystem::path path(subStrings.at(1));
									
									if (const auto texture = Resources::Get<Graphics::Texture>(path.stem().string()).lock()) {
										_output->m_Roughness_Texture = texture;
									}
								}
							}
							else if (key == "map_Pm") {
								
								if (subStrings.size() >= 2) {
								
									const std::filesystem::path path(subStrings.at(1));
									
									if (const auto texture = Resources::Get<Graphics::Texture>(path.stem().string()).lock()) {
										_output->m_Metallic_Texture = texture;
									}
								}
							}
							else if (key == "map_Ke") {
								
								if (subStrings.size() >= 2) {
								
									const std::filesystem::path path(subStrings.at(1));
									
									if (const auto texture = Resources::Get<Graphics::Texture>(path.stem().string()).lock()) {
										_output->m_Emission_Texture = texture;
									}
								}
							}
							else if (key == "norm") {
								
								if (subStrings.size() >= 2) {
								
									const std::filesystem::path path(subStrings.at(1));
									
									if (const auto texture = Resources::Get<Graphics::Texture>(path.stem().string()).lock()) {
										_output->m_Normal_Texture = texture;
									}
								}
							}
							else if (key == "map_Ao") {
								
								if (subStrings.size() >= 2) {
								
									const std::filesystem::path path(subStrings.at(1));
									
									if (const auto texture = Resources::Get<Graphics::Texture>(path.stem().string()).lock()) {
										_output->m_AO_Texture = texture;
									}
								}
							}
						}
					}
					
					fs.close();
					
					result = true;
					
					Debug::Log("Done.", LogType::Info);
				}
				else {
					throw std::runtime_error("Couldn't open filestream.");
				}
			}
			catch (const std::exception& e) {
				Debug::Log("Failed.", LogType::Error);
				Debug::Log(e);
			}
			
			return result;
		}
		
		static bool TryLoad(const std::array<std::filesystem::path, 6>& _paths, std::shared_ptr<Graphics::Cubemap>& _output,
				const Graphics::Texture::Parameters::Format&     _format,
				const Graphics::Texture::Parameters::FilterMode& _filterMode,
				const Graphics::Texture::Parameters::WrapMode&   _wrapMode
			) {
			
			bool result = false;
			
			Debug::Log("Loading Cubemap... ", LogType::Info, true);
			
			try {
				
				_output.reset(
					new Graphics::Cubemap(-1, -1, 0, _format, _filterMode, _wrapMode)
				);
				
				// de Vries, J. (n.d.). LearnOpenGL - Cubemaps. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-OpenGL/Cubemaps [Accessed 15 Dec. 2023].
				
				glGenTextures(1, &_output->m_TextureID);
				
				if (_output->m_TextureID != GL_NONE) {
					
					int cubemap_resolution = -1;
					
					Graphics::Cubemap::Bind(*_output);
					
					for (size_t i = 0; i < _paths.size(); ++i) {
						
						glm::ivec2 loaded_resolution { -1, -1 };
						
						int channels;
						GLenum texture_format;
						
						Graphics::Texture::GetFormatData(_format.PixelFormat(), texture_format, channels);
						
						void* data;
						GLenum data_format;
				
						if (strcmp(_paths[i].extension().string().c_str(), ".hdr") == 0 ||
						    strcmp(_paths[i].extension().string().c_str(), ".exr") == 0
						) {
							data_format = GL_FLOAT;
							data = stbi_loadf(
								_paths[i].string().c_str(),
								&loaded_resolution.x,
								&loaded_resolution.y,
								nullptr,
								_output->Format().Channels()
							);
						}
						else {
							data_format = GL_UNSIGNED_BYTE;
							data = stbi_load(
								_paths[i].string().c_str(),
								&loaded_resolution.x,
								&loaded_resolution.y,
								nullptr,
								_output->Format().Channels()
							);
						}
						
						cubemap_resolution = std::max(
							std::max(
								loaded_resolution.x,
								loaded_resolution.y
							),
							cubemap_resolution
						);
						
						if (data != nullptr) {
							
							glTexImage2D(
								GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
								0,
								static_cast<GLint>(_output->Format().PixelFormat()),
								loaded_resolution.x,
								loaded_resolution.y,
								0,
								_output->Format().TextureFormat(),
								data_format,
								data
							);
							
							stbi_image_free(data);
						}
						else {
							throw std::runtime_error("Failed to load texture at path \"" + _paths[i].string() + "\".");
						}
					}
					
					if (_format.Mips()) {
						glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
						
						const auto min = _output->FilterMode().Min();
						
						switch (min) {
							case GL_NEAREST: {
								glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
								break;
							}
							case GL_LINEAR:  {
								glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
								break;
							}
							case GL_NEAREST_MIPMAP_NEAREST:
							case GL_NEAREST_MIPMAP_LINEAR:
							case GL_LINEAR_MIPMAP_NEAREST:
							case GL_LINEAR_MIPMAP_LINEAR: {
								glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min));
								break;
							}
							default: {
								Debug::Log("Unknown (possibly unsupported) mipmap filtering value \"" + std::to_string(min) + "\". ", LogType::Warning);
								
								glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min));
								break;
							}
						}
					}
					
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(_output->FilterMode().Mag()));
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,     static_cast<GLint>(_output->WrapMode().WrapS()));
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,     static_cast<GLint>(_output->WrapMode().WrapT()));
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,     static_cast<GLint>(_output->WrapMode().WrapR()));
					
					glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
					
					_output->m_Width  = cubemap_resolution;
					_output->m_Height = cubemap_resolution;
				}
				
				result = true;
				
				Debug::Log("Done.", LogType::Info);
			}
			catch (const std::exception& e) {
				Debug::Log("Failed.", LogType::Error);
				Debug::Log(e);
			}
			
			return result;
		}
		
		static bool TryLoad(const std::filesystem::path& _path, std::shared_ptr<Graphics::Shader>& _output) {
	
			bool result = false;
			
			try {
				_output.reset(new Graphics::Shader(_path), [](Graphics::Shader* _ptr) { delete _ptr; });
	
				result = true;
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
			
			return result;
		}
		
		static bool TryLoad(const std::vector<Graphics::SubShader>& _subshaders, std::shared_ptr<Graphics::Shader>& _output) {
			
			bool result = false;
			
			try {
				_output.reset(new Graphics::Shader(_subshaders), [](Graphics::Shader* _ptr) { delete _ptr; });
	
				result = true;
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
			
			return result;
		}
		
		
	public:
	
		/**
		 * @brief Initialises the Resources system.
		 */
		static void Init()  {
			IndexDependencies();
			IndexAssets();
		}
		
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
		static std::weak_ptr<T> Get(const std::string& _name, const bool& _fallback = true) noexcept {
			
			std::weak_ptr<T> result;
			
			try {
				
				auto& item = const_cast<Asset<T>&>(GetBucket<T>()[_name]);
				
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
	inline void Resources::Asset<Audio::AudioClip>::Load() {

		try {
			
			if (exists(m_Path)) {
				
				if (Resources::TryLoad(m_Path, m_Item)) {
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
				
				if (Resources::TryLoad(m_Path, m_Item)) {
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
				
				if (Resources::TryLoad(m_Path, m_Item)) {
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
				
				if (Resources::TryLoad(m_Path, m_Item, { format, true }, { GL_LINEAR, GL_LINEAR }, { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE })) {
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
					
					if (Resources::TryLoad(faces, m_Item, { GL_RGB32F, true }, { GL_LINEAR, GL_LINEAR }, { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE })) {
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
				
				if (Resources::TryLoad(m_Path, m_Item)) {
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
			
			auto item = Resources::GetBucket<Graphics::Cubemap>().Get(_name).value_or({ _name });
			
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