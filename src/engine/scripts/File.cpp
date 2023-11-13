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
	
	bool File::TryLoad(const std::filesystem::path& _path, Texture& _output, GLenum _format = GL_RGBA, bool _generateMipmaps = true) {
		
		bool result = false;
		
		try {
			
			glGenTextures(1, &_output.m_TextureID);
			
			if (_output.m_TextureID > 0) {
				
				int channels;
				GLenum texture_format;
				
				Texture::GetFormatData(_format, texture_format, channels);
				
				void* data;
				GLenum data_format;
				
				if (strcmp(_path.extension().c_str(), ".hdr") == 0) {
					data_format = GL_FLOAT;
					data = stbi_loadf(
						_path.c_str(),
						&_output.m_Width,
						&_output.m_Height,
						nullptr,
						channels
					);
				}
				else {
					data_format = GL_UNSIGNED_BYTE;
					data = stbi_load(
						_path.c_str(),
						&_output.m_Width,
						&_output.m_Height,
						nullptr,
						channels
					);
				}
				
				if (data != nullptr) {
				
					Texture::Bind(_output);
					
					glTexImage2D(
						GL_TEXTURE_2D,
						0,
						_format,
						_output.m_Width,
						_output.m_Height,
						0,
						texture_format,
						   data_format,
						   data
					);
					
					stbi_image_free(data);
					
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
					
					if (_generateMipmaps) {
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

						glGenerateMipmap(GL_TEXTURE_2D);
					}
					else {
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					}
					
					Texture::Unbind();
					
					result = true;
				}
				else {
					throw std::runtime_error("Failed to load texture at path \"" + _path.string() + "\".");
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
		
		if (_output == nullptr) {
			_output.reset(new Mesh());
		}
		
		/*
		 * Implementation derived from Mesh.cpp and Mesh.h
		 * provided with OBJLoader_v3 project in GCP labs.
		 */
		
		try {
			
			// Find file
			std::ifstream inputFile(_path);
			
			if (inputFile.is_open()) {
				
				// OBJ files can store texture coordinates, positions and normals
				std::vector<glm::vec2> rawUVData;
				std::vector<glm::vec3> rawPositionData;
				std::vector<glm::vec3> rawNormalData;
				
				std::vector<glm::vec2> orderedUVData;
				std::vector<glm::vec3> orderedPositionData;
				std::vector<glm::vec3> orderedNormalData;
				
				std::string currentLine;
				
				while (std::getline(inputFile, currentLine)) {
					
					std::stringstream currentLineStream(currentLine);
					//std::cout<< currentLine <<std::endl;
					
					if (!currentLine.substr(0, 2).compare(0, 2, "vt")) {
						std::string junk;
						float x, y;
						currentLineStream >> junk >> x >> y;
						rawUVData.push_back(glm::vec2(x, y));
					}
					else if (!currentLine.substr(0, 2).compare(0, 2, "vn")) {
						std::string junk;
						float x, y, z;
						currentLineStream >> junk >> x >> y >> z;
						rawNormalData.push_back(glm::vec3(x, y, z));
					}
					else if (!currentLine.substr(0, 2).compare(0, 1, "v")) {
						std::string junk;
						float x, y, z;
						currentLineStream >> junk >> x >> y >> z;
						rawPositionData.push_back(glm::vec3(x, y, z));
					}
					else if (!currentLine.substr(0, 2).compare(0, 1, "f")) {
						std::string junk;
						std::string verts[4];
						
						currentLineStream >> junk >> verts[0] >> verts[1] >> verts[2] >> verts[3];
						
						if (verts[3].empty()) {
							
							for (unsigned int i = 0; i < 3; i++) {
								
								std::stringstream currentSection(verts[i]);
								
								// There is just position data
								unsigned int posID = 0;
								unsigned int uvID = 0;
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
									orderedPositionData.push_back(rawPositionData[posID - 1]);
								}
								if (uvID > 0) {
									orderedUVData.push_back(rawUVData[uvID - 1]);
								}
								if (normID > 0) {
									orderedNormalData.push_back(rawNormalData[normID - 1]);
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
				
				_output->m_VertexCount = orderedPositionData.size();
				
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
					glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _output->m_VertexCount * 3, &orderedPositionData[0], GL_STATIC_DRAW);
					
					// This tells OpenGL how we link the vertex data to the shader
					// (We will look at this properly in the lectures)
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
					glEnableVertexAttribArray(0);
					
					if (orderedNormalData.size() > 0) {
						
						// Create a generic 'buffer'
						glGenBuffers(1, &_output->m_NormalVBO_ID);
						// Tell OpenGL that we want to activate the buffer and that it's a VBO
						glBindBuffer(GL_ARRAY_BUFFER, _output->m_NormalVBO_ID);
						// With this buffer active, we can now send our data to OpenGL
						// We need to tell it how much data to send
						// We can also tell OpenGL how we intend to use this buffer - here we say GL_STATIC_DRAW because we're only writing it once
						glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _output->m_VertexCount * 3, &orderedNormalData[0], GL_STATIC_DRAW);
						
						// This tells OpenGL how we link the vertex data to the shader
						// (We will look at this properly in the lectures)
						glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
						glEnableVertexAttribArray(1);
					}
					
					if (orderedUVData.size() > 0) {
						
						// Create a generic 'buffer'
						glGenBuffers(1, &_output->m_TexCoordVBO_ID);
						// Tell OpenGL that we want to activate the buffer and that it's a VBO
						glBindBuffer(GL_ARRAY_BUFFER, _output->m_TexCoordVBO_ID);
						// With this buffer active, we can now send our data to OpenGL
						// We need to tell it how much data to send
						// We can also tell OpenGL how we intend to use this buffer - here we say GL_STATIC_DRAW because we're only writing it once
						glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _output->m_VertexCount * 2, &orderedUVData[0], GL_STATIC_DRAW);
						
						// This tells OpenGL how we link the vertex data to the shader
						// (We will look at this properly in the lectures)
						glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
						glEnableVertexAttribArray(2);
					}
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
		throw std::runtime_error("Not implemented!");
	}
	
	bool File::TryLoad(const std::filesystem::path& _path, std::shared_ptr<Shader>& _output) {
	
	
	}
	
	Cubemap File::Load(const std::array<std::filesystem::path, 6>& _paths, GLenum _format = GL_RGBA, bool _generateMipmaps = true) {
		
		Cubemap result;
		
		glGenTextures(1, &result.m_TextureID);
		
		if (result.m_TextureID > 0) {
			
			Cubemap::Bind(result);
			
			int cubemap_resolution = -1;
			
			for (int i = 0; i < _paths.size(); ++i) {
				
				try {
					
					int channels;
					GLenum texture_format;
					
					Texture::GetFormatData(_format, texture_format, channels);
					
					glm::ivec2 loaded_resolution = { -1, -1 };
					
					void* data;
					GLenum data_format;
					
					if (strcmp(_paths[i].extension().c_str(), ".hdr") == 0) {
						data_format = GL_FLOAT;
						data = stbi_loadf(
							_paths[i].c_str(),
							&loaded_resolution.x,
							&loaded_resolution.y,
							nullptr,
							channels
						);
					}
					else {
						data_format = GL_UNSIGNED_BYTE;
						data = stbi_load(
							_paths[i].c_str(),
							&loaded_resolution.x,
							&loaded_resolution.y,
							nullptr,
							channels
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
							_format,
							loaded_resolution.x,
							loaded_resolution.y,
							0,
							texture_format,
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