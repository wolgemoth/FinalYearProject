#pragma once

#ifndef FINALYEARPROJECT_MODEL_H
#define FINALYEARPROJECT_MODEL_H

namespace LouiEriksson {
	
	class Mesh {
	
		friend class File;
		
	private:
		
		GLuint m_VAO_ID;
		
		GLuint m_PositionVBO_ID;
		GLuint m_TexCoordVBO_ID;
		GLuint   m_NormalVBO_ID;
		GLuint  m_TangentVBO_ID;
		
		int m_VertexCount;

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
		
		~Mesh();
		
		[[nodiscard]] GLuint         VAO_ID() const;
		[[nodiscard]] GLuint PositionVBO_ID() const;
		[[nodiscard]] GLuint TexCoordVBO_ID() const;
		[[nodiscard]] GLuint   NormalVBO_ID() const;
		[[nodiscard]] GLuint  TangentVBO_ID() const;
		
		[[nodiscard]] unsigned long VertexCount() const;
	};
}

#endif //FINALYEARPROJECT_MODEL_H