#pragma once

#ifndef FINALYEARPROJECT_MODEL_H
#define FINALYEARPROJECT_MODEL_H

#include "../../external/wavefront/include/wavefront/wavefront.h"

namespace LouiEriksson {
	
	class Mesh {
	
	private:
		
		WfModel m_Mesh;
	
		Mesh();
		
	public:
		
		struct Quad {

			// Every screen and texture coordinate for every vertex in the mesh.
			static constexpr glm::vec4 s_VertexData[] {
				glm::vec4(-1.0f, -1.0f, 0.0f, 0.0f),
				glm::vec4( 1.0f, -1.0f, 1.0f, 0.0f),
				glm::vec4(-1.0f,  1.0f, 0.0f, 1.0f),
				glm::vec4(-1.0f,  1.0f, 0.0f, 1.0f),
				glm::vec4( 1.0f, -1.0f, 1.0f, 0.0f),
				glm::vec4( 1.0f,  1.0f, 1.0f, 1.0f),
			};
	
			// Number of vertices in the mesh.
			static constexpr unsigned long s_VertexCount = sizeof(s_VertexData);
		};
		
		struct Cube {
			
			// Define the vertices of the cube
		    static constexpr glm::vec3 s_Vertices[] = {
		        glm::vec3(-0.5f, -0.5f,  0.5f),
		        glm::vec3( 0.5f, -0.5f,  0.5f),
		        glm::vec3( 0.5f,  0.5f,  0.5f),
		        glm::vec3(-0.5f,  0.5f,  0.5f),
		        glm::vec3(-0.5f, -0.5f, -0.5f),
		        glm::vec3( 0.5f, -0.5f, -0.5f),
		        glm::vec3( 0.5f,  0.5f, -0.5f),
		        glm::vec3(-0.5f,  0.5f, -0.5f)
		    };
		
		    // Define the texture coordinates of the cube
		    static constexpr glm::vec2 s_TexCoords[] = {
		        glm::vec2(0.0f, 0.0f),
		        glm::vec2(1.0f, 0.0f),
		        glm::vec2(1.0f, 1.0f),
		        glm::vec2(0.0f, 1.0f),
		        glm::vec2(1.0f, 0.0f),
		        glm::vec2(0.0f, 0.0f),
		        glm::vec2(0.0f, 1.0f),
		        glm::vec2(1.0f, 1.0f)
		    };
		
		    // Define the indices of the cube
		    static constexpr GLuint s_Indices[] = {
		        0, 1, 2, // Front
		        2, 3, 0,
		        4, 5, 6, // Back
		        6, 7, 4,
		        7, 3, 0, // Left
		        0, 4, 7,
		        1, 5, 6, // Right
		        6, 2, 1,
		        3, 2, 6, // Top
		        6, 7, 3,
		        0, 1, 5, // Bottom
		        5, 4, 0
		    };
	
			// Number of vertices in the mesh.
			static constexpr unsigned long s_VertexCount = sizeof(s_Vertices);
		};
		
		~Mesh();
		
		/// <summary> TryLoad a mesh from a path. </summary>
		static std::shared_ptr<Mesh> Load(const std::string& _path);
		
		[[nodiscard]] GLuint PositionVBO_ID() const;
		[[nodiscard]] GLuint TexCoordVBO_ID() const;
		[[nodiscard]] GLuint   NormalVBO_ID() const;
		[[nodiscard]] GLuint         VAO_ID() const;
		[[nodiscard]] GLuint     Texture_ID() const;
		
		[[nodiscard]] unsigned long VertexCount() const;
	};
}

#endif //FINALYEARPROJECT_MODEL_H