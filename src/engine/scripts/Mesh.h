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