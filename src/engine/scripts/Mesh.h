#pragma once

#ifndef FINALYEARPROJECT_MODEL_H
#define FINALYEARPROJECT_MODEL_H

namespace LouiEriksson {
	
	class Mesh {
	
	private:
	
		WfModel m_Mesh;
	
	public:
	
		[[maybe_unused]] [[nodiscard]] GLuint PositionVBO_ID() const;
		[[maybe_unused]] [[nodiscard]] GLuint TexCoordVBO_ID() const;
		[[maybe_unused]] [[nodiscard]] GLuint   NormalVBO_ID() const;
		[[maybe_unused]] [[nodiscard]] GLuint         VAO_ID() const;
		[[maybe_unused]] [[nodiscard]] GLuint     Texture_ID() const;
		[[maybe_unused]] [[nodiscard]]    int    VertexCount() const;
	
		 Mesh();
		~Mesh();
	
		/// <summary> Load a mesh from a path. </summary>
		static std::shared_ptr<Mesh> Load(const char* _path);
	};
}

#endif //FINALYEARPROJECT_MODEL_H