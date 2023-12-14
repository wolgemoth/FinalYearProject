#include "stdafx.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include "File.h"
#include "Resources.h"

namespace LouiEriksson {
	
	std::string File::ReadAllText(const std::filesystem::path& _path) {
	
		std::stringstream result;
	
		std::fstream fs;
		fs.open(_path, std::ios::in);
	
		if (fs.is_open()) {
	
			std::string line;
	
			while (std::getline(fs, line)) {
				result << line << "\n";
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
				append = (((uint)_type & (uint)File::Directory::EntryType::DIRECTORY) != 0);
			}
			else {
				append = (((uint)_type & (uint)File::Directory::EntryType::FILE) != 0);
			}
			
			if (append) { result.push_back(item); }
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
				subDirectories.push_back(item);
				
				append = (((uint)_type & (uint)File::Directory::EntryType::DIRECTORY) != 0);
			}
			else {
				append = (((uint)_type & (uint)File::Directory::EntryType::FILE) != 0);
			}
			
			// Append entries of requested type to result.
			if (append) { result.push_back(item); }
		}
		
		// Iterate through all subdirectories recursively and append output to result.
		for (const auto& subDirectory : subDirectories) {
		
			auto items = GetEntriesRecursive(subDirectory, _type);
			
			result.insert(result.end(), items.begin(), items.end());
		}
		
		return result;
	}
	
	bool File::TryLoad(const std::filesystem::path& _path, std::shared_ptr<Texture>& _output, GLenum _format = GL_RGBA, bool _generateMipmaps = true) {
		
		bool result = false;
		
		std::cout << "Loading Texture Asset \"" << _path.c_str() << "\"... ";
		
		try {
			
			_output.reset(new Texture(-1, -1, 0, Texture::Parameters::Format(_format, _generateMipmaps), Texture::Parameters::FilterMode(GL_LINEAR, GL_LINEAR), Texture::Parameters::WrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE)));
			
			glGenTextures(1, &_output->m_TextureID);
			
			if (_output->m_TextureID > 0) {
				
				int channels;
				
				GLenum texture_format;
				
				Texture::GetFormatData(_format, texture_format, channels);
				
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
				
					Texture::Bind(*_output);
					
					glTexImage2D(
						GL_TEXTURE_2D,
						0,
						static_cast<GLint>(_format),
						_output->m_Width,
						_output->m_Height,
						0,
						texture_format,
						   data_format,
						   data
					);
					
					stbi_image_free(data);
					
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					
					if (_generateMipmaps) {
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

						glGenerateMipmap(GL_TEXTURE_2D);
					}
					else {
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					}
					
					Texture::Unbind();
					
					result = true;
					
					std::cout << "Done.\n";
				}
				else {
					throw std::runtime_error("Failed.");
				}
			}
		}
		catch (const std::exception& e) {
			std::cout << e.what() << "\n";
		}
		
		return result;
	}
	
	bool File::TryLoad(const std::filesystem::path& _path, std::shared_ptr<Mesh>& _output) {
		
		bool result = false;
		
		_output.reset(new Mesh());
		
		/*
		 * Implementation derived from Mesh.cpp and Mesh.h
		 * provided with OBJLoader_v3 project in GCP labs.
		 */
		
		try {
			
			// Find file
			std::ifstream inputFile(_path);
			
			if (inputFile.is_open()) {
				
				// OBJ files can store texture coordinates, positions and normals
				std::vector<glm::vec3> rawVertices;
				std::vector<glm::vec2> rawUVs;
				std::vector<glm::vec3> rawNormals;
				
				std::vector<glm::vec3> vertices;
				std::vector<glm::vec3> normals;
				std::vector<glm::vec2> UVs;
				
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
							
							for (unsigned int i = 0; i < 3; i++) {
								
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
									char junk;
									currentSection >> posID >> junk >> junk >> normID;
								}
								else {
									char junk;
									currentSection >> posID >> junk >> uvID >> junk >> normID;
								}
								
								if (posID > 0) {
									vertices.push_back(rawVertices[posID - 1]);
								}
								if (uvID > 0) {
									UVs.push_back(rawUVs[uvID - 1]);
								}
								if (normID > 0) {
									normals.push_back(rawNormals[normID - 1]);
								}
							}
						}
						else {
							
							inputFile.close();
							
							throw std::runtime_error("WARNING: This OBJ loader only works with triangles but a quad has been detected. Please triangulate your mesh.\n");
						}
					}
				}
				
				inputFile.close();
				
				_output->m_VertexCount = vertices.size();
				
				if (_output->m_VertexCount > 0) {
					
					glGenVertexArrays(1, &(_output->m_VAO_ID));
		
					glBindVertexArray(_output->m_VAO_ID);
					
					// Create a generic 'buffer'
					glGenBuffers(1, &_output->m_PositionVBO_ID);
					// Tell OpenGL that we want to activate the buffer and that it's a VBO
					glBindBuffer(GL_ARRAY_BUFFER, _output->m_PositionVBO_ID);
					// With this buffer active, we can now send our data to OpenGL
					// We need to tell it how much data to send
					// We can also tell OpenGL how we intend to use this buffer - here we say GL_STATIC_DRAW because we're only writing it once
					glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(float) * _output->m_VertexCount * 3), vertices.data(), GL_STATIC_DRAW);
					
					// This tells OpenGL how we link the vertex data to the shader
					// (We will look at this properly in the lectures)
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
					glEnableVertexAttribArray(0);
					
					if (!normals.empty()) {
						
						// Create a generic 'buffer'
						glGenBuffers(1, &_output->m_NormalVBO_ID);
						// Tell OpenGL that we want to activate the buffer and that it's a VBO
						glBindBuffer(GL_ARRAY_BUFFER, _output->m_NormalVBO_ID);
						// With this buffer active, we can now send our data to OpenGL
						// We need to tell it how much data to send
						// We can also tell OpenGL how we intend to use this buffer - here we say GL_STATIC_DRAW because we're only writing it once
						glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(float) * _output->m_VertexCount * 3), normals.data(), GL_STATIC_DRAW);
						
						// This tells OpenGL how we link the vertex data to the shader
						// (We will look at this properly in the lectures)
						glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
						glEnableVertexAttribArray(1);
					}
					
					if (!UVs.empty()) {
						
						// Create a generic 'buffer'
						glGenBuffers(1, &_output->m_TexCoordVBO_ID);
						// Tell OpenGL that we want to activate the buffer and that it's a VBO
						glBindBuffer(GL_ARRAY_BUFFER, _output->m_TexCoordVBO_ID);
						// With this buffer active, we can now send our data to OpenGL
						// We need to tell it how much data to send
						// We can also tell OpenGL how we intend to use this buffer - here we say GL_STATIC_DRAW because we're only writing it once
						glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(float) * _output->m_VertexCount * 2), UVs.data(), GL_STATIC_DRAW);
						
						// This tells OpenGL how we link the vertex data to the shader
						// (We will look at this properly in the lectures)
						glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
						glEnableVertexAttribArray(2);
					}
					
					// If the mesh has both texture coordinates and normals, compute the tangents and bitangents.
					if (!normals.empty() && !UVs.empty()) {
					
						// https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
						
						std::vector<glm::vec3>   tangents;
						std::vector<glm::vec3> bitangents;
						
						for (int i = 0; i < vertices.size(); i += 3) {
						
							// Shortcuts for vertices
					        const auto& pos0 = vertices[  i  ];
					        const auto& pos1 = vertices[i + 1];
					        const auto& pos2 = vertices[i + 2];
					
					        // Shortcuts for UVs
					        const auto& uv0 = UVs[  i  ];
					        const auto& uv1 = UVs[i + 1];
					        const auto& uv2 = UVs[i + 2];
							
							const auto delta_pos1 = pos1 - pos0;
						    const auto delta_pos2 = pos2 - pos0;
							
						    const auto delta_texcoord1 = uv1 - uv0;
						    const auto delta_texcoord2 = uv2 - uv0;
						
							float f = 1.0f / (delta_texcoord1.x * delta_texcoord2.y - delta_texcoord2.x * delta_texcoord1.y);
						    const auto   tangent = f * (delta_pos1 * delta_texcoord2.y - delta_pos2 * delta_texcoord1.y);
						    const auto bitangent = f * (delta_pos2 * delta_texcoord1.x - delta_pos1 * delta_texcoord2.x);
							
							// Set the same tangent for all three vertices of the triangle.
					        // They will be merged later, in vboindexer.cpp
					        tangents.push_back(tangent);
					        tangents.push_back(tangent);
					        tangents.push_back(tangent);
					
					        // Same thing for bitangents
					        bitangents.push_back(bitangent);
					        bitangents.push_back(bitangent);
					        bitangents.push_back(bitangent);
						}
						
						// TODO: Perform tangent averaging / smoothing.
						
					    glGenBuffers(1, &_output->m_TangentVBO_ID);
					    glBindBuffer(GL_ARRAY_BUFFER, _output->m_TangentVBO_ID);
					    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(tangents.size() * sizeof(tangents[0])), tangents.data(), GL_STATIC_DRAW);
					
						glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
						glEnableVertexAttribArray(3);
						
					    glGenBuffers(1, &_output->m_BitangentVBO_ID);
					    glBindBuffer(GL_ARRAY_BUFFER, _output->m_BitangentVBO_ID);
					    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(bitangents.size() * sizeof(bitangents[0])), bitangents.data(), GL_STATIC_DRAW);
						
						glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
						glEnableVertexAttribArray(4);
					}
					
					glBindVertexArray(0);
	                glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
				
				result = true;
			}
			else {
				
				std::stringstream ss;
					ss << "WARNING: File not found: " << _path << "\n";
				
				throw std::runtime_error(ss.str());
			}
		}
		catch (const std::exception& e) {
			std::cout << e.what() << "\n";
		}
		
		return result;
	}
	
	bool File::TryLoad(const std::filesystem::path& _path, std::shared_ptr<Material>& _output) {
		
		bool result = false;
		
		std::cout << "Loading Material \"" << _path.c_str() << "\"... ";
		
		try {
			
			_output.reset(new Material());
			
			std::fstream fs;
			fs.open(_path, std::ios::in);
		
			if (fs.is_open()) {
		
				std::string line;
		
				while (std::getline(fs, line)) {
					
					auto subStrings = Utils::Split(line, ' ');
					
					if (!subStrings.empty()) {
						
						const auto& key = subStrings.at(0);
						
						if (key == "map_Kd") {
							
							if (subStrings.size() >= 2) {
							
								const std::filesystem::path path(subStrings.at(1));
								
								std::shared_ptr<Texture> texture;
								if (Resources::TryGetTexture(path.stem().string(), texture)) {
									_output->m_Albedo = texture;
								}
							}
						}
						else if (key == "map_Ks") {
							std::cout << "Specular map loading not implemented. ";
						}
						else if (key == "map_Tr" || key == "map_d") {
							std::cout << "Transparency map loading not implemented. ";
						}
						else if (key == "bump" || key == "map_bump" || key == "bm") {
							std::cout << "Bump map loading not implemented. ";
						}
						else if (key == "disp") {
							
							if (subStrings.size() >= 2) {
							
								const std::filesystem::path path(subStrings.at(1));
								
								std::shared_ptr<Texture> texture;
								if (Resources::TryGetTexture(path.stem().string(), texture)) {
									_output->m_Displacement = texture;
								}
							}
						}
						else if (key == "map_Pr") {
							
							if (subStrings.size() >= 2) {
							
								const std::filesystem::path path(subStrings.at(1));
								
								std::shared_ptr<Texture> texture;
								if (Resources::TryGetTexture(path.stem().string(), texture)) {
									_output->m_Roughness = texture;
								}
							}
						}
						else if (key == "map_Pm") {
							
							if (subStrings.size() >= 2) {
							
								const std::filesystem::path path(subStrings.at(1));
								
								std::shared_ptr<Texture> texture;
								if (Resources::TryGetTexture(path.stem().string(), texture)) {
									_output->m_Metallic = texture;
								}
							}
						}
						else if (key == "map_Ke") {
							
							if (subStrings.size() >= 2) {
							
								const std::filesystem::path path(subStrings.at(1));
								
								std::shared_ptr<Texture> texture;
								if (Resources::TryGetTexture(path.stem().string(), texture)) {
									_output->m_Emission = texture;
								}
							}
						}
						else if (key == "norm") {
							
							if (subStrings.size() >= 2) {
							
								const std::filesystem::path path(subStrings.at(1));
								
								std::shared_ptr<Texture> texture;
								if (Resources::TryGetTexture(path.stem().string(), texture)) {
									_output->m_Normals = texture;
								}
							}
						}
						else if (key == "map_Ao") {
							
							if (subStrings.size() >= 2) {
							
								const std::filesystem::path path(subStrings.at(1));
								
								std::shared_ptr<Texture> texture;
								if (Resources::TryGetTexture(path.stem().string(), texture)) {
									_output->m_AO = texture;
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
			
			std::cout << e.what() << "\n";
		}
		
		return result;
	}
	
	bool File::TryLoad(const std::array<std::filesystem::path, 6>& _paths, std::shared_ptr<Cubemap>& _output, GLenum _format = GL_RGBA, bool _generateMipmaps = true) {
		
		bool result = false;
		
		_output.reset(
			new Cubemap(
				-1, -1, 0,
				Texture::Parameters::Format(_format, _generateMipmaps),
				Texture::Parameters::FilterMode(GL_LINEAR, GL_LINEAR),
				Texture::Parameters::WrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE)
			)
		);
		
		try {
			
			glGenTextures(1, &_output->m_TextureID);
			
			if (_output->m_TextureID > 0) {
				
				Cubemap::Bind(*_output);
				
				int cubemap_resolution = -1;
				
				for (int i = 0; i < _paths.size(); ++i) {
					
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
						std::cout << e.what() << "\n";
					}
				}
				
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(_output->FilterMode().Mag()));
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,     static_cast<GLint>(_output->WrapMode().WrapS()));
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,     static_cast<GLint>(_output->WrapMode().WrapT()));
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,     static_cast<GLint>(_output->WrapMode().WrapR()));
				
				if (_generateMipmaps) {
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				
					glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
				}
				else {
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(_output->FilterMode().Min()));
				}
				
				glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
				
				_output->m_Width  = cubemap_resolution;
				_output->m_Height = cubemap_resolution;
				
				Cubemap::Unbind();
			}
			
			result = true;
		}
		catch (const std::exception& _e) {
			std::cout << _e.what() << "\n";
		}
		
		return result;
	}
}