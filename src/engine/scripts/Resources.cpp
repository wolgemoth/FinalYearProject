#include "Resources.h"

namespace LouiEriksson {
	
	void Resources::PreloadMeshes() {
		std::cout << "Please implement PreloadMeshes() function!\n";
	}
	
	void Resources::PreloadMaterials() {
		std::cout << "Please implement PreloadMaterials() function!\n";
	}
	
	void Resources::PreloadTextures() {
		std::cout << "Please implement PreloadTextures() function!\n";
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
		
			std::vector<Shader::SubShader> shader;
			
			for (const auto& subshader : kvp.second) {
				shader.emplace_back(Shader::SubShader(subshader.first.c_str(), subshader.second));
			}
			
			shaders.push_back(shader);
			
			// Compile shader and add to cache.
			auto compiled = Shader::Create(shader);
			m_Shaders.Add(compiled->Name(), compiled);
		}
	}
	
	void Resources::Preload() {
		PreloadMeshes();
		PreloadMaterials();
		PreloadTextures();
		PreloadShaders();
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
	
	std::shared_ptr<Mesh> Resources::GetMesh(const std::string& _name) {
		return m_Meshes.Return(_name);
	}
	
	std::shared_ptr<Material> Resources::GetMaterial(const std::string& _name) {
		return m_Materials.Return(_name);
	}
	
	std::shared_ptr<Texture> Resources::GetTexture(const std::string& _name) {
		return m_Textures.Return(_name);
	}
	
	std::shared_ptr<Shader> Resources::GetShader(const std::string& _name) {
		return m_Shaders.Return(_name);
	}
	
} // LouiEriksson