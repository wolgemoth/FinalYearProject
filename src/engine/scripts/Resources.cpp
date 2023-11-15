#include "Resources.h"

namespace LouiEriksson {
	
	void Resources::PreloadMeshes() {
		std::cout << "Please implement PreloadMeshes() function!\n";
	}
	
	void Resources::PreloadMaterials() {
		
		Hashmap<std::string, std::filesystem::path> files;
		
		for (const auto& item : File::Directory::GetEntriesRecursive(m_MaterialsDirectory, File::Directory::EntryType::FILE)) {
			
			if (strcmp(item.extension().c_str(), ".mtl") == 0) {
				files.Add(item.stem().string(), item);
			}
		}
		
		for (const auto& kvp : files.GetAll()) {
			
			std::shared_ptr<Material> material;
			if (File::TryLoad(kvp.second, material)) {
				m_Materials.Add(kvp.first, material);
			}
		}
	}
	
	void Resources::PreloadTextures() {
		
		Hashmap<std::string, std::filesystem::path> files;
		
		for (const auto& item : File::Directory::GetEntriesRecursive(m_TexturesDirectory, File::Directory::EntryType::FILE)) {
			files.Add(item.stem().string(), item);
		}
		
		for (const auto& kvp : files.GetAll()) {
			
			std::shared_ptr<Texture> texture;
			if (File::TryLoad(kvp.second, texture, GL_RGB32F, true)) {
				m_Textures.Add(kvp.first, texture);
			}
		}
	}
	
	void Resources::PreloadShaders() {
	
		Hashmap<std::string, std::vector<std::pair<std::filesystem::path, GLenum>>> files;
		
		for (const auto& item : File::Directory::GetEntriesRecursive(m_ShadersDirectory, File::Directory::EntryType::FILE)) {
			
			std::vector<std::pair<std::filesystem::path, GLenum>> subshaders;
			
			files.Get(item.stem().string(), subshaders);
			
			GLenum shaderType;
			
			     if (strcmp(item.extension().c_str(), ".vert") == 0) { shaderType =   GL_VERTEX_SHADER; }
			else if (strcmp(item.extension().c_str(), ".frag") == 0) { shaderType = GL_FRAGMENT_SHADER; }
			else if (strcmp(item.extension().c_str(), ".geom") == 0) { shaderType = GL_GEOMETRY_SHADER; }
			else {
				shaderType = GL_NONE;
			}
			
			if (shaderType != GL_NONE) {
				subshaders.push_back({item, shaderType});
				
				files.Assign(item.stem().string(), subshaders);
			}
		}
		
		std::vector<std::vector<Shader::SubShader>> shaders;
		
		for (const auto& kvp : files.GetAll()) {
		
			std::vector<Shader::SubShader> subShaders;
			
			for (const auto& subshader : kvp.second) {
				subShaders.emplace_back(Shader::SubShader(subshader.first.c_str(), subshader.second));
			}
			
			shaders.push_back(subShaders);
			
			// Compile shader and add to cache.
			auto compiled = std::shared_ptr<Shader>(new Shader(subShaders), [](Shader* _ptr) { delete _ptr; });
			
			m_Shaders.Add(compiled->Name(), compiled);
		}
	}
	
	void Resources::Preload() {
		PreloadShaders();
		PreloadMeshes();
		PreloadTextures();
		PreloadMaterials();
	}
	
	bool Resources::TryGetMesh(const std::string& _name, std::shared_ptr<Mesh>& _output) {
		return m_Meshes.Get(_name, _output);
	}
	
	bool Resources::TryGetMaterial(const std::string& _name, std::shared_ptr<Material>& _output) {
		return m_Materials.Get(_name, _output);
	}
	
	bool Resources::TryGetTexture(const std::string& _name, std::shared_ptr<Texture>& _output) {
		return m_Textures.Get(_name, _output);
	}
	
	bool Resources::TryGetShader(const std::string& _name, std::shared_ptr<Shader>& _output) {
		return m_Shaders.Get(_name, _output);
	}
	
	std::weak_ptr<Mesh> Resources::GetMesh(const std::string& _name) {
		return m_Meshes.Return(_name);
	}
	
	std::weak_ptr<Material> Resources::GetMaterial(const std::string& _name) {
		return m_Materials.Return(_name);
	}
	
	std::weak_ptr<Texture> Resources::GetTexture(const std::string& _name) {
		return m_Textures.Return(_name);
	}
	
	std::weak_ptr<Shader> Resources::GetShader(const std::string& _name) {
		return m_Shaders.Return(_name);
	}
	
} // LouiEriksson