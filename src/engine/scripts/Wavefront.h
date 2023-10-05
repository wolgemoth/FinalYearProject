#include "stdafx.h"

#include "Mesh.h"
#include "Material.h"

#pragma once

#ifndef FINALYEARPROJECT_WAVEFRONT_H
#define FINALYEARPROJECT_WAVEFRONT_H

namespace LouiEriksson {
	
	class Wavefront {
	
	public:
		
		static std::shared_ptr<Mesh> LoadMesh(std::string _path) {
			
			Mesh* result = new Mesh();
			
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
					
					if (!currentLine.substr(0, 2).compare(0, 2, "vt")) {
						
						std::string junk;
						
						float x, y;
						
						currentLineStream >> junk >> x >> y;
						rawUVData.push_back(glm::vec2(x, y));
					} else if (!currentLine.substr(0, 2).compare(0, 2, "vn")) {
						
						std::string junk;
						
						float x, y, z;
						currentLineStream >> junk >> x >> y >> z;
						rawNormalData.push_back(glm::vec3(x, y, z));
					} else if (!currentLine.substr(0, 2).compare(0, 1, "v")) {
						
						std::string junk;
						
						float x, y, z;
						currentLineStream >> junk >> x >> y >> z;
						rawPositionData.push_back(glm::vec3(x, y, z));
					} else if (!currentLine.substr(0, 2).compare(0, 1, "f")) {
						
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
								} else if (verts[i].find("//") != std::string::npos) {
									// No texcoords
									char junk;
									currentSection >> posID >> junk >> junk >> normID;
								} else {
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
						} else if (!currentLine.substr(0, 2).compare(0, 1, "mtllib")) {
						
						} else {
							
							std::cerr << "WARNING: This OBJ loader only works with triangles but a quad has been detected. Please triangulate your mesh."
							          << std::endl;
							
							inputFile.close();
							
							orderedPositionData.clear();
							orderedUVData.clear();
							orderedNormalData.clear();
						}
					}
				}
				
				inputFile.close();
				
				result->m_VertexCount = orderedPositionData.size();
				
				if (result->m_VertexCount > 0) {
					
					glBindVertexArray(result->m_VAO_ID);
					
					// Create a generic 'buffer'
					glGenBuffers(1, &result->m_PositionVBO_ID);
					// Tell OpenGL that we want to activate the buffer and that it's a VBO
					glBindBuffer(GL_ARRAY_BUFFER, result->m_PositionVBO_ID);
					// With this buffer active, we can now send our data to OpenGL
					// We need to tell it how much data to send
					// We can also tell OpenGL how we intend to use this buffer - here we say GL_STATIC_DRAW because we're only writing it once
					glBufferData(GL_ARRAY_BUFFER, sizeof(float) * result->m_VertexCount * 3, &orderedPositionData[0], GL_STATIC_DRAW);
					
					// This tells OpenGL how we link the vertex data to the shader
					// (We will look at this properly in the lectures)
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
					glEnableVertexAttribArray(0);
					
					if (orderedNormalData.size() > 0) {
						
						// Create a generic 'buffer'
						glGenBuffers(1, &result->m_NormalVBO_ID);
						// Tell OpenGL that we want to activate the buffer and that it's a VBO
						glBindBuffer(GL_ARRAY_BUFFER, result->m_NormalVBO_ID);
						// With this buffer active, we can now send our data to OpenGL
						// We need to tell it how much data to send
						// We can also tell OpenGL how we intend to use this buffer - here we say GL_STATIC_DRAW because we're only writing it once
						glBufferData(GL_ARRAY_BUFFER, sizeof(float) * result->m_VertexCount * 3, &orderedNormalData[0], GL_STATIC_DRAW);
						
						// This tells OpenGL how we link the vertex data to the shader
						// (We will look at this properly in the lectures)
						glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
						glEnableVertexAttribArray(1);
					}
					
					if (orderedUVData.size() > 0) {
						
						// Create a generic 'buffer'
						glGenBuffers(1, &result->m_Texture_ID);
						// Tell OpenGL that we want to activate the buffer and that it's a VBO
						glBindBuffer(GL_ARRAY_BUFFER, result->m_Texture_ID);
						// With this buffer active, we can now send our data to OpenGL
						// We need to tell it how much data to send
						// We can also tell OpenGL how we intend to use this buffer - here we say GL_STATIC_DRAW because we're only writing it once
						glBufferData(GL_ARRAY_BUFFER, sizeof(float) * result->m_VertexCount * 2, &orderedUVData[0], GL_STATIC_DRAW);
						
						// This tells OpenGL how we link the vertex data to the shader
						// (We will look at this properly in the lectures)
						glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
						glEnableVertexAttribArray(2);
					}
					
				}
				
			} else {
				std::cerr << "WARNING: File not found: " << _path << std::endl;
			}
			
			return std::shared_ptr<Mesh>(result);
			
		}
		
//		static std::shared_ptr<Material> LoadMaterial(std::string _path) {
//
//			Material* result = new Material();
//
//			return std::shared_ptr<Material>(result);
//		}
	
	};
	
}

#endif //FINALYEARPROJECT_WAVEFRONT_H