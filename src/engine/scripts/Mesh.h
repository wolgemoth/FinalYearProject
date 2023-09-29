#pragma once

#ifndef _MODEL_H
#define _MODEL_H

class Mesh {

private:

	WfModel m_Mesh;

public:

	GLuint PositionVBO_ID();
	GLuint TexCoordVBO_ID();
	GLuint   NormalVBO_ID();
	GLuint         VAO_ID();
	GLuint     Texture_ID();
	   int    VertexCount();

	 Mesh();
	~Mesh();

	/// <summary> Load a mesh from a path. </summary>
	static std::shared_ptr<Mesh> Load(const char* _path);
};

#endif // !_MODEL_H
