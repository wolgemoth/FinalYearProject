#include "stdafx.h"

#include "File.h"

namespace LouiEriksson {
	
	std::string File::ReadAllText(const std::filesystem::path& _path) {
	
		std::stringstream result;
	
		std::fstream fstream;
		fstream.open(_path, std::ios::in);
	
		if (fstream.is_open()) {
	
			std::string line;
	
			while (std::getline(fstream, line)) {
				result << line << "\n";
			}
			fstream.close();
		}
	
		return result.str();
	}
	
	std::filesystem::path File::AsPath(const char* _path) {
		return { _path };
	}
	
	std::filesystem::path File::AsPath(const std::string& _path) {
		return { _path };
	}
	
	bool File::TryLoad(const std::filesystem::path& _path, Texture& _output, bool _generateMipmaps = true) {
		
		bool result = false;
		
		try {
			
			int channels = -1;
			
			auto* data = stbi_load(
				_path.c_str(),
				&_output.m_Width,
				&_output.m_Height,
				&channels,
				0
			);
			
			if (data != nullptr) {
				
				glGenTextures(1, &_output.m_TextureID);
				
				if (_output.m_TextureID > 0) {
					
					Texture::Bind(_output);
					
					glTexImage2D(
						GL_TEXTURE_2D,
						0,
						GL_RGBA,
						_output.m_Width,
						_output.m_Height,
						0,
						GL_RGBA,
						GL_UNSIGNED_BYTE,
						data
					);
					
					if (_generateMipmaps) {
						glGenerateMipmap(GL_TEXTURE_2D);
					}
					
					Texture::Unbind();
					
					result = true;
				}
				
				stbi_image_free(data);
			}
			else {
				throw std::runtime_error("Failed to load texture at path \"" + _path.string() + "\".");
			}
		}
		catch (const std::exception& e) {
			std::cout << e.what() << "\n";
		}
		
		return result;
	}
	
	Cubemap File::Load(const std::array<std::filesystem::path, 6>& _paths, bool _generateMipmaps) {
		
		Cubemap result;
		
		glGenTextures(1, &result.m_TextureID);
		
		if (result.m_TextureID > 0) {
			
			Cubemap::Bind(result);
			
			int cubemap_resolution = -1;
			
			for (int i = 0; i < _paths.size(); ++i) {
				
				try {
				
					int channels = -1;
					
					glm::ivec2 loaded_resolution = { -1, -1 };
					
					auto* data = stbi_load(
						_paths[i].c_str(),
						&loaded_resolution.x,
						&loaded_resolution.y,
						&channels,
						0
					);
					
					cubemap_resolution = glm::max(
							glm::max(
							loaded_resolution.x,
							loaded_resolution.y
						),
							cubemap_resolution
					);
					
					if (data != nullptr) {
						
						glTexImage2D(
							GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
							0,
							GL_RGBA,
							loaded_resolution.x,
							loaded_resolution.y,
							0,
							GL_RGBA,
							GL_UNSIGNED_BYTE,
							data
						);
						
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
						
						if (_generateMipmaps) {
							glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
						}
						
						stbi_image_free(data);
					}
					else {
						throw std::runtime_error("Failed to load texture at path \"" + _paths[i].string() + "\".");
					}
				}
				catch (const std::exception& e) {
					std::cout << e.what() << "\n";
				}
			}
			
			result.m_Width  = cubemap_resolution;
			result.m_Height = cubemap_resolution;
			
			Cubemap::Unbind();
		}
		
		return result;
	}
}