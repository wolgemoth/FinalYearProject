#include "File.h"

#include "../audio/AudioClip.h"
#include "../core/utils/Utils.h"
#include "../graphics/Material.h"
#include "../graphics/Mesh.h"
#include "../graphics/Shader.h"
#include "../graphics/Texture.h"
#include "../graphics/textures/Cubemap.h"

#include "Resources.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

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
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace LouiEriksson::Engine {
	
	struct PackedVertex {
		
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
		
		bool operator<(const PackedVertex _other) const {
			return memcmp((void*)this, (void*)&_other, sizeof(PackedVertex))>0;
		};
	};
	
	bool getSimilarVertexIndex_fast(const PackedVertex& packed, std::map<PackedVertex, GLuint>& VertexToOutIndex, GLuint& result){
		
		const auto it     = VertexToOutIndex.find(packed);
		const auto output = it != VertexToOutIndex.end();
		
		if (output) {
			result = it->second;
		}
		
		return output;
	}
	
	std::string File::ReadAllText(const std::filesystem::path& _path) {
	
		std::stringstream result;
	
		std::fstream fs;
		fs.open(_path, std::ios::in);
	
		if (fs.is_open()) {
	
			std::string line;
	
			while (std::getline(fs, line)) {
				result << line<< '\n';
			}
			fs.close();
		}
	
		return result.str();
	}
	
	std::vector<std::filesystem::path> File::Directory::GetEntries(const std::filesystem::path& _path, const File::Directory::EntryType& _type) {
	
		std::vector<std::filesystem::path> result;
		
		const std::filesystem::directory_iterator itr(_path);
		
		for (const auto& item : itr) {
			
			bool append;
			
			if (item.is_directory()) {
				append = (((unsigned int)_type & (unsigned int)File::Directory::EntryType::DIRECTORY) != 0);
			}
			else {
				append = (((unsigned int)_type & (unsigned int)File::Directory::EntryType::FILE) != 0);
			}
			
			if (append) { result.emplace_back(item); }
		}
		
		return result;
	}
	
	std::vector<std::filesystem::path> File::Directory::GetEntriesRecursive(const std::filesystem::path& _path, const File::Directory::EntryType& _type) {
	
		std::vector<std::filesystem::path> result;
		std::vector<std::filesystem::path> subDirectories;
		
		// Get all entries.
		for (const auto& item : GetEntries(_path, (File::Directory::EntryType)(File::Directory::EntryType::FILE | File::Directory::EntryType::DIRECTORY))) {
			
			bool append;
			
			if (is_directory(item)) {
			
				// Append all subdirectories to a vector.
				subDirectories.emplace_back(item);
				
				append = (((unsigned int)_type & (unsigned int)File::Directory::EntryType::DIRECTORY) != 0);
			}
			else {
				append = (((unsigned int)_type & (unsigned int)File::Directory::EntryType::FILE) != 0);
			}
			
			// Append entries of requested type to result.
			if (append) { result.emplace_back(item); }
		}
		
		// Iterate through all subdirectories recursively and append output to result.
		for (const auto& subDirectory : subDirectories) {
		
			auto items = GetEntriesRecursive(subDirectory, _type);
			
			result.insert(result.end(), items.begin(), items.end());
		}
		
		return result;
	}
	
	bool File::TryLoad(const std::filesystem::path& _path, std::shared_ptr<Audio::AudioClip>& _output) {
		
		bool result = false;
		
		std::cout << "Loading AudioClip \"" << _path.c_str() << "\"... ";
		
		try {
			_output.reset(new Audio::AudioClip(_path));
			
			std::cout << "Done.\n";
			
			result = true;
		}
		catch (const std::exception& e) {
			std::cout << "Failed.\n\n";
			
			std::cerr << e.what() << std::endl;
		}
		
		return result;
	}
	
	bool File::TryLoad(const std::filesystem::path& _path, std::shared_ptr<Graphics::Texture>& _output,
		const Graphics::Texture::Parameters::Format&     _format,
		const Graphics::Texture::Parameters::FilterMode& _filterMode,
		const Graphics::Texture::Parameters::WrapMode&   _wrapMode
	) {
		
		bool result = false;
		
		std::cout << "Loading Texture \"" << _path.c_str() << "\"... ";
		
		try {
			
			_output.reset(
				new Graphics::Texture(-1, -1, 0, _format, _filterMode, _wrapMode)
			);
			
			glGenTextures(1, &_output->m_TextureID);
			
			if (_output->m_TextureID > 0) {
				
				int channels;
				GLenum texture_format;
				
				Graphics::Texture::GetFormatData(_format.PixelFormat(), texture_format, channels);
				
				void* data;
				GLenum data_format;
				
				if (strcmp(_path.extension().string().c_str(), ".hdr") == 0) {
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
								
								std::cout << "Unknown (possibly unsupported) mipmap filtering value \"" << min << "\".";
								
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min));
								break;
							}
						}
					}
					else {
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _output->FilterMode().Min());
					}
					
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _output->FilterMode().Mag());
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					
					Graphics::Texture::Unbind();
					
					Utils::GLDumpError();
					
					result = true;
					
					std::cout << "Done.\n";
				}
				else {
					throw std::runtime_error("Failed.");
				}
			}
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		
		return result;
	}
	
	bool File::TryLoad(const std::filesystem::path& _path, std::shared_ptr<Graphics::Mesh>& _output) {
		
		bool result = false;
		
		std::cout << "Loading Mesh \"" << _path.c_str() << "\"... ";
		
		/*
		 * Implementation derived from Mesh.cpp and Mesh.h
		 * provided with OBJLoader_v3 project in GACP labs.
		 *
		 * I added tangent calculations to the code derived from an implementation by Learn OpenGL:
         * de Vries, J. (n.d.). LearnOpenGL - Normal Mapping. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Normal-Mapping [Accessed 15 Dec. 2023].
		 */
		
		try {
			
			// Find file
			std::ifstream inputFile(_path);
			
			if (inputFile.is_open()) {
				
				// OBJ files can store texture coordinates, positions and normals
				std::vector<GLuint> indices;
				
				std::vector<glm::vec3> vertices;
				std::vector<glm::vec3> normals;
				std::vector<glm::vec2> UVs;
				
				{
					std::vector<glm::vec3> rawVertices;
					std::vector<glm::vec2> rawUVs;
					std::vector<glm::vec3> rawNormals;
					
					std::string currentLine;
					
					while (std::getline(inputFile, currentLine)) {
						
						std::stringstream currentLineStream(currentLine);
						
						if (currentLine.substr(0, 2).compare(0, 2, "vt") == 0) {
							
							std::string junk;
							
							float x, y;
							
							currentLineStream >> junk >> x >> y;
							rawUVs.emplace_back(x, y);
						}
						else if (currentLine.substr(0, 2).compare(0, 2, "vn") == 0) {
							
							std::string junk;
							
							float x, y, z;
							
							currentLineStream >> junk >> x >> y >> z;
							rawNormals.emplace_back(x, y, z);
						}
						else if (currentLine.substr(0, 2).compare(0, 1, "v") == 0) {
							
							std::string junk;
							
							float x, y, z;
							
							currentLineStream >> junk >> x >> y >> z;
							rawVertices.emplace_back(x, y, z);
						}
						else if (currentLine.substr(0, 2).compare(0, 1, "f") == 0) {
							
							std::string junk;
							std::string verts[4];
							
							currentLineStream >> junk >> verts[0] >> verts[1] >> verts[2] >> verts[3];
							
							if (verts[3].empty()) {
								
								for (unsigned int i = 0; i < 3; ++i) {
									
									std::stringstream currentSection(verts[i]);
									
									// There is just position data
									unsigned int  posID = 0;
									unsigned int   uvID = 0;
									unsigned int normID = 0;
									
									if (verts[i].find('/') == std::string::npos) {
										// No texcoords or normals
										currentSection >> posID;
									}
									else if (verts[i].find("//") != std::string::npos) {
										// No texcoords
										char junk2;
										currentSection >> posID >> junk2 >> junk2 >> normID;
									}
									else {
										char junk2;
										currentSection >> posID >> junk2 >> uvID >> junk2 >> normID;
									}
									
									if (posID > 0) {
										vertices.emplace_back(rawVertices[posID - 1]);
									}
									if (uvID > 0) {
										UVs.emplace_back(rawUVs[uvID - 1]);
									}
									if (normID > 0) {
										normals.emplace_back(rawNormals[normID - 1]);
									}
								}
							}
							else {
								inputFile.close();
								
								throw std::runtime_error("WARNING: This OBJ loader only works with triangles but a quad has been detected. Please triangulate your mesh.\n");
							}
						}
					}
				}
				
				inputFile.close();

				std::vector<glm::vec3> out_vertices;
				std::vector<glm::vec3> out_normals;
				std::vector<glm::vec2> out_uvs;
				
				std::map<PackedVertex, GLuint> VertexToOutIndex;
				
				for (auto i = 0; i < vertices.size(); ++i){

					const PackedVertex packed = { vertices[i], UVs[i], normals[i] };
					
					// Try to find a similar vertex in out_XXXX
					GLuint index;
					const auto found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);
					
					if (found) { // A similar vertex is already in the VBO, use it instead !
						indices.emplace_back(index);
					}
					else { // If not, it needs to be added in the output data.
						out_vertices.emplace_back(vertices[i]);
						out_uvs     .emplace_back(     UVs[i]);
						out_normals .emplace_back( normals[i]);
						
						auto itm = out_vertices.size() - 1;
						
						indices.emplace_back(itm);
						
						VertexToOutIndex[packed] = itm;
					}
				}
				
				_output = Graphics::Mesh::Create(out_vertices, indices, out_normals, out_uvs, true, GL_TRIANGLES);
				
				std::cout << "Done.\n";
				
				result = true;
			}
			else {
				
				std::stringstream ss;
					ss << "WARNING: File not found: " << _path<< '\n';
				
				throw std::runtime_error(ss.str());
			}
		}
		catch (const std::exception& e) {
			std::cout << "Failed.\n\n";
			
			std::cerr << e.what() << std::endl;
		}
		
		return result;
	}
	
	bool File::TryLoad(const std::filesystem::path& _path, std::shared_ptr<Graphics::Material>& _output) {
		
		bool result = false;
		
		std::cout << "Loading Material \"" << _path.c_str() << "\"... ";
		
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
							std::cout << "Ambient color loading not implemented... ";
						}
						else if (key == "Kd") {
							
							auto r = std::stof(subStrings.at(1));
							auto g = std::stof(subStrings.at(2));
							auto b = std::stof(subStrings.at(3));
							
							_output->m_Albedo_Color = { r, g, b, 1.0 };
							
							try {
								_output->m_Albedo_Color.a = std::stof(subStrings.at(4));
							}
							catch (...) {}
						}
						else if (key == "d" || key == "Tr") {
							_output->m_Albedo_Color.a = 1.0 - std::clamp(std::stof(subStrings.at(1)), 0.0f, 1.0f);
						}
						else if (key == "Ks") {
							std::cout << "Specular color loading not implemented... ";
						}
						else if (key == "illum") {
							std::cout << "Support for different lighting models is not implemented... ";
						}
						else if (key == "Ao") {
							_output->m_AO = std::stof(subStrings.at(1));
						}
						else if (key == "Ns") {
							_output->m_Roughness = 1.0 - (std::atan(std::stof(subStrings.at(1))) / (M_PI / 2));
						}
						else if (key == "Ke") {
							
							_output->m_Emission_Color = {
									std::stof(subStrings.at(1)),
									std::stof(subStrings.at(2)),
									std::stof(subStrings.at(3))
							};
						}
						else if (key == "Ni") {
							std::cout << "Optical density loading not implemented... ";
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
							std::cout << "Specular map loading not implemented... ";
						}
						else if (key == "map_Tr" || key == "map_d") {
							std::cout << "Transparency map loading not implemented... ";
						}
						else if (key == "bump" || key == "map_bump" || key == "bm") {
							std::cout << "Bump map loading not implemented... ";
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
				
				std::cout << "Done.\n";
			}
			else {
				throw std::runtime_error("Couldn't open filestream.");
			}
		}
		catch (const std::exception& e) {
			
			std::cout << "Failed.\n";
			
			std::cerr << e.what() << std::endl;
		}
		
		return result;
	}
	
	bool File::TryLoad(const std::array<std::filesystem::path, 6>& _paths, std::shared_ptr<Graphics::Cubemap>& _output, GLenum _format, bool _generateMipmaps) {
		
		bool result = false;
		
		_output.reset(
			new Graphics::Cubemap(
				-1, -1, 0,
				Graphics::Texture::Parameters::Format(_format, _generateMipmaps),
				Graphics::Texture::Parameters::FilterMode(GL_LINEAR, GL_LINEAR),
				Graphics::Texture::Parameters::WrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE)
			)
		);
		
		try {
			
			// de Vries, J. (n.d.). LearnOpenGL - Cubemaps. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-OpenGL/Cubemaps [Accessed 15 Dec. 2023].
			
			glGenTextures(1, &_output->m_TextureID);
			
			if (_output->m_TextureID > 0) {
				
				Graphics::Cubemap::Bind(*_output);
				
				int cubemap_resolution = -1;
				
				for (auto i = 0; i < _paths.size(); ++i) {
					
					try {
						
						glm::ivec2 loaded_resolution = { -1, -1 };
						
						void* data;
						GLenum data_format;
						
						if (strcmp(_paths[i].extension().string().c_str(), ".hdr") == 0) {
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
					catch (const std::exception& e) {
						std::cerr << e.what() << std::endl;
					}
				}
				
				if (_generateMipmaps) {
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
							
							std::cout << "Unknown (possibly unsupported) mipmap filtering value \"" << min << "\".";
							
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
				
				Graphics::Cubemap::Unbind();
			}
			
			result = true;
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		
		return result;
	}
	
	bool File::TryLoad(const std::filesystem::path& _path, std::shared_ptr<Graphics::Shader>& _output) {

		bool result = false;
		
		try {
			_output.reset(new Graphics::Shader(_path), [](Graphics::Shader* _ptr) { delete _ptr; });

			result = true;
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		
		return result;
	}
	
	bool File::TryLoad(const std::vector<Graphics::SubShader>& _subshaders, std::shared_ptr<Graphics::Shader>& _output) {
		
		bool result = false;
		
		try {
			_output.reset(new Graphics::Shader(_subshaders), [](Graphics::Shader* _ptr) { delete _ptr; });

			result = true;
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		
		return result;
	}
	
} // LouiEriksson::Engine