#include "Resources.h"

#include "../core/utils/Hashmap.h"

#include "Debug.h"
#include "File.h"

#include <GL/glew.h>

#include <cstring>
#include <exception>
#include <filesystem>
#include <memory>
#include <string>
#include <typeindex>
#include <utility>
#include <vector>

namespace LouiEriksson::Engine {
	
	bool Resources::GetType(const std::string& _extension, std::type_index& _output) {
		
		const static Hashmap<std::string, std::type_index> lookup {
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
		
		return lookup.Get(_extension, _output);
	}
	
	void Resources::IndexAssets() {
		
		for (const auto& item : File::Directory::GetEntriesRecursive("assets/", File::Directory::EntryType::FILE)) {
		
			if (item.has_extension()) {
			
				std::type_index type { typeid(void) };
				if (GetType(item.extension(), type)) {
					   
					     if (type == typeid(   Audio::AudioClip)) {     m_Audio.Assign(item.stem().string(), { item }); }
					else if (type == typeid(Graphics::Material )) { m_Materials.Assign(item.stem().string(), { item }); }
					else if (type == typeid(Graphics::Mesh     )) {    m_Meshes.Assign(item.stem().string(), { item }); }
					else if (type == typeid(Graphics::Shader   )) {   m_Shaders.Assign(item.stem().string(), { item }); }
					else if (type == typeid(Graphics::Texture  )) {  m_Textures.Assign(item.stem().string(), { item }); }
				}
				else {
					Debug::Log("Unable to determine the type of asset with extension " + item.extension().string(), LogType::Warning);
				}
			}
		}
	}
	
	void Resources::IndexDependencies() {
		
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
					const auto contents = File::ReadAllText(dependency);
					
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
	
	void Resources::Init() {
		IndexDependencies();
		IndexAssets();
	}
	
} // LouiEriksson::Engine