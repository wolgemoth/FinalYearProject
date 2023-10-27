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
	
	bool File::TryLoad(const std::filesystem::path& _path, Texture& _output, GLenum _format = GL_RGBA, bool _generateMipmaps = true) {
		
		bool result = false;
		
		try {
			
			int desired_channels = 0;
			
			switch (_format) {
				case GL_R:    { desired_channels = 1; break; }
				case GL_RG:   { desired_channels = 2; break; }
				case GL_RGB:  { desired_channels = 3; break; }
				case GL_RGBA: { desired_channels = 4; break; }
				default: {
					std::cout << "Unknown texture format \"" << _format << "\"\n";
				}
			}
			
			auto* data = stbi_load(
				_path.c_str(),
				&_output.m_Width,
				&_output.m_Height,
				NULL,
				desired_channels
			);
			
			if (data != nullptr) {
				
				glGenTextures(1, &_output.m_TextureID);
				
				if (_output.m_TextureID > 0) {
					
					Texture::Bind(_output);
					
					glTexImage2D(
						GL_TEXTURE_2D,
						0,
						_format,
						_output.m_Width,
						_output.m_Height,
						0,
						_format,
						GL_UNSIGNED_BYTE,
						data
					);
					
					if (_generateMipmaps) {
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

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
	
	Cubemap File::Load(const std::array<std::filesystem::path, 6>& _paths, GLenum _format = GL_RGBA, bool _generateMipmaps = true) {
		
		Cubemap result;
		
		glGenTextures(1, &result.m_TextureID);
		
		if (result.m_TextureID > 0) {
			
			Cubemap::Bind(result);
			
			int cubemap_resolution = -1;
			
			for (int i = 0; i < _paths.size(); ++i) {
				
				try {
				
					int desired_channels = 0;
					
					switch (_format) {
						case GL_R:    { desired_channels = 1; break; }
						case GL_RG:   { desired_channels = 2; break; }
						case GL_RGB:  { desired_channels = 3; break; }
						case GL_RGBA: { desired_channels = 4; break; }
						default: {
							std::cout << "Unknown texture format \"" << _format << "\"\n";
						}
					}
					
					glm::ivec2 loaded_resolution = { -1, -1 };
					
					auto* data = stbi_load(
						_paths[i].c_str(),
						&loaded_resolution.x,
						&loaded_resolution.y,
						NULL,
						desired_channels
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
							_format,
							loaded_resolution.x,
							loaded_resolution.y,
							0,
							_format,
							GL_UNSIGNED_BYTE,
							data
						);
						
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
			
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			
			if (_generateMipmaps) {
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			
				glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			}
			else {
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}
			
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
			
			result.m_Width  = cubemap_resolution;
			result.m_Height = cubemap_resolution;
			
			Cubemap::Unbind();
		}
		
		return result;
	}
}