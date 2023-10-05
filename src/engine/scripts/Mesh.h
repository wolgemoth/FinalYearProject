#pragma once

#ifndef FINALYEARPROJECT_MODEL_H
#define FINALYEARPROJECT_MODEL_H

namespace LouiEriksson {
	
	class Mesh {
	
		friend class Wavefront;
		
	private:
		
		Mesh();
		
		GLuint m_PositionVBO_ID;
		GLuint m_TexCoordVBO_ID;
		GLuint   m_NormalVBO_ID;
		GLuint         m_VAO_ID;
		GLuint     m_Texture_ID;
		
		unsigned long m_VertexCount;
		
	public:
		
		~Mesh();
		
		/// <summary> Load a mesh from a path. </summary>
		[[maybe_unused]] static std::shared_ptr<Mesh> Load(const std::string& _path);
		
		[[maybe_unused]] [[nodiscard]] GLuint PositionVBO_ID() const;
		[[maybe_unused]] [[nodiscard]] GLuint TexCoordVBO_ID() const;
		[[maybe_unused]] [[nodiscard]] GLuint   NormalVBO_ID() const;
		[[maybe_unused]] [[nodiscard]] GLuint         VAO_ID() const;
		[[maybe_unused]] [[nodiscard]] GLuint     Texture_ID() const;
		
		[[maybe_unused]] [[nodiscard]] unsigned long VertexCount() const;
	};
}

#endif //FINALYEARPROJECT_MODEL_H