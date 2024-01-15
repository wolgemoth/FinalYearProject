#include "Resources.h"

#include "../audio/AudioClip.h"
#include "../core/utils/Hashmap.h"
#include "../graphics/Material.h"
#include "../graphics/Mesh.h"
#include "../graphics/Shader.h"
#include "../graphics/Texture.h"

#include "File.h"

#include <GL/glew.h>

#include <cstring>
#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// @Assessor: This class was submitted for GEP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	void Resources::PreloadAudio() {
		
		Hashmap<std::string, std::filesystem::path> files;
		
		for (const auto& item : File::Directory::GetEntriesRecursive(m_AudioDirectory, File::Directory::EntryType::FILE)) {
			files.Add(item.stem().string(), item);
		}
		
		for (const auto& kvp : files.GetAll()) {
			
			std::shared_ptr<Audio::AudioClip> clip;
			if (File::TryLoad(kvp.second, clip)) {
				m_Audio.Add(kvp.first, clip);
			}
		}
	}
	
	void Resources::PreloadMeshes() {
		
		Hashmap<std::string, std::filesystem::path> files;
		
		for (const auto& item : File::Directory::GetEntriesRecursive(m_MeshesDirectory, File::Directory::EntryType::FILE)) {
			
			if (strcmp(item.extension().string().c_str(), ".obj") == 0) {
				files.Add(item.stem().string(), item);
			}
		}
		
		for (const auto& kvp : files.GetAll()) {
			
			std::shared_ptr<Graphics::Mesh> mesh;
			if (File::TryLoad(kvp.second, mesh)) {
				m_Meshes.Add(kvp.first, mesh);
			}
		}
	}
	
	void Resources::PreloadMaterials() {
		
		Hashmap<std::string, std::filesystem::path> files;
		
		for (const auto& item : File::Directory::GetEntriesRecursive(m_MaterialsDirectory, File::Directory::EntryType::FILE)) {
			
			if (strcmp(item.extension().string().c_str(), ".mtl") == 0) {
				files.Add(item.stem().string(), item);
			}
		}
		
		for (const auto& kvp : files.GetAll()) {
			
			std::shared_ptr<Graphics::Material> material;
			if (File::TryLoad(kvp.second, material)) {
				m_Materials.Add(kvp.first, material);
			}
		}
	}
	
	void Resources::PreloadTextures() {
		
		/* PRELOAD SRGB */
		{
			Hashmap<std::string, std::filesystem::path> files;
			
			for (const auto& item : File::Directory::GetEntriesRecursive(m_TexturesSRGBDirectory, File::Directory::EntryType::FILE)) {
				files.Add(item.stem().string(), item);
			}
			
			for (const auto& kvp : files.GetAll()) {
				
				std::shared_ptr<Graphics::Texture> texture;
				if (File::TryLoad(kvp.second, texture, GL_SRGB, true)) {
					m_Textures.Add(kvp.first, texture);
				}
			}
		}
		
		/* PRELOAD LINEAR */
		{
			Hashmap<std::string, std::filesystem::path> files;
			
			for (const auto& item : File::Directory::GetEntriesRecursive(m_TexturesLinearDirectory, File::Directory::EntryType::FILE)) {
				files.Add(item.stem().string(), item);
			}
			
			for (const auto& kvp : files.GetAll()) {
				
				std::shared_ptr<Graphics::Texture> texture;
				if (File::TryLoad(kvp.second, texture, GL_RGB32F, true)) {
					m_Textures.Add(kvp.first, texture);
				}
			}
		}
	}
	
	void Resources::PreloadShaders() {
		
		/* INCLUDE SHADER DEPENDENCIES */
		{
			std::vector<std::filesystem::path> dependencies;
			
			for (const auto& item : File::Directory::GetEntriesRecursive(m_ShaderIncludeDirectory, File::Directory::EntryType::FILE)) {
				
				if (strcmp(item.extension().string().c_str(), ".glsl") == 0) {
					dependencies.emplace_back(item);
				}
			}
			
			for (const auto& dependency : dependencies) {
			
				std::cout << "Loading Shader Dependency \"" << dependency.c_str() << "\"... " << std::flush;
				
				try {
					const auto name = "/" + dependency.string();
					const auto contents = File::ReadAllText(dependency);
					
					// https://www.opengl.org/registry/specs/ARB/shading_language_include.txt
					glNamedStringARB(
						GL_SHADER_INCLUDE_ARB,
						    static_cast<GLint>(name.length()),
						    name.c_str(),
						static_cast<GLint>(contents.length()),
						contents.c_str()
					);
					
					std::cout << "Done.\n";
				}
				catch (const std::exception& e) {
					
					std::cout << "Failed.\n";
					
					std::cout << e.what()<< '\n';
				}
			}
		}
		
		/* PRELOAD + COMPILE SHADERS */
		{
			// Container for shaders where the subshaders are all in separate files.
			Hashmap<std::string, std::vector<std::pair<std::filesystem::path, GLenum>>> separated;
			
			// Container for shaders where the subshaders are all in the same file.
			std::vector<std::filesystem::path> combined;
			
			for (const auto& item : File::Directory::GetEntriesRecursive(m_ShaderProgramsDirectory, File::Directory::EntryType::FILE)) {
				
				GLenum shaderType;
				
				     if (strcmp(item.extension().string().c_str(), ".vert") == 0) { shaderType =   GL_VERTEX_SHADER; }
				else if (strcmp(item.extension().string().c_str(), ".frag") == 0) { shaderType = GL_FRAGMENT_SHADER; }
				else if (strcmp(item.extension().string().c_str(), ".geom") == 0) { shaderType = GL_GEOMETRY_SHADER; }
				else if (strcmp(item.extension().string().c_str(), ".glsl") == 0) { shaderType =          GL_SHADER; }
				else {
					shaderType = GL_NONE;
				}
				
				if (shaderType == GL_SHADER) {
					combined.emplace_back(item);
				}
				else if (shaderType != GL_NONE) {
					
					std::vector<std::pair<std::filesystem::path, GLenum>> subshaders;
					separated.Get(item.stem().string(), subshaders);
				
					subshaders.emplace_back(item, shaderType);
					
					separated.Assign(item.stem().string(), subshaders);
				}
			}
			
			// Iterate over all of the shaders defined by subshaders in
			// different files, compiling them and adding to the cache.
			{
				for (const auto& kvp : separated.GetAll()) {
				
					std::vector<Graphics::Shader::SubShader> subShaders;
					subShaders.reserve(kvp.second.size());
					
					for (const auto& subshader : kvp.second) {
						subShaders.emplace_back(subshader.first.string().c_str(), subshader.second);
					}
					
					// Compile shader and add to cache.
					auto compiled = std::shared_ptr<Graphics::Shader>(new Graphics::Shader(subShaders), [](Graphics::Shader* _ptr) { delete _ptr; });
					m_Shaders.Add(compiled->Name(), compiled);
				}
			}
			
			// Iterate over all the shaders defined by subshaders in the same
			// file, compiling the programs and adding them to the cache.
			{
				for (const auto& item : combined) {
				
					// Compile shader and add to cache.
					auto compiled = std::shared_ptr<Graphics::Shader>(new Graphics::Shader(item), [](Graphics::Shader* _ptr) { delete _ptr; });
					m_Shaders.Add(compiled->Name(), compiled);
				}
			}
		}
	}
	
	void Resources::Preload() {
		PreloadAudio();
		PreloadShaders();
		PreloadMeshes();
		PreloadTextures();
		PreloadMaterials();
	}
	
	bool Resources::TryGetAudio(const std::string& _name, std::shared_ptr<Audio::AudioClip>& _output) noexcept {
		return m_Audio.Get(_name, _output);
	}
	
	bool Resources::TryGetMesh(const std::string& _name, std::shared_ptr<Graphics::Mesh>& _output) noexcept {
		return m_Meshes.Get(_name, _output);
	}
	
	bool Resources::TryGetMaterial(const std::string& _name, std::shared_ptr<Graphics::Material>& _output) noexcept {
		return m_Materials.Get(_name, _output);
	}
	
	bool Resources::TryGetTexture(const std::string& _name, std::shared_ptr<Graphics::Texture>& _output) noexcept {
		return m_Textures.Get(_name, _output);
	}
	
	bool Resources::TryGetShader(const std::string& _name, std::shared_ptr<Graphics::Shader>& _output) noexcept {
		return m_Shaders.Get(_name, _output);
	}
	
	const std::weak_ptr<Audio::AudioClip> Resources::GetAudio(const std::string& _name) {
		return m_Audio.Return(_name);
	}
	
	const std::weak_ptr<Graphics::Mesh> Resources::GetMesh(const std::string& _name) {
		return m_Meshes.Return(_name);
	}
	
	const std::weak_ptr<Graphics::Material> Resources::GetMaterial(const std::string& _name) {
		return m_Materials.Return(_name);
	}
	
	const std::weak_ptr<Graphics::Texture> Resources::GetTexture(const std::string& _name) {
		return m_Textures.Return(_name);
	}
	
	const std::weak_ptr<Graphics::Shader> Resources::GetShader(const std::string& _name) {
		return m_Shaders.Return(_name);
	}
	
} // LouiEriksson