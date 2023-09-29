#pragma once

#ifndef _RENDERER_H
#define _RENDERER_H

#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

class Renderer : public Component {

private:

	/// <summary> Mesh of the Renderer. </summary>
	std::shared_ptr<Mesh>      m_Mesh;

	/// <summary> Material of the Renderer. </summary>
	std::shared_ptr<Material>  m_Material;

	/// <summary> Transform of the Renderer. </summary>
	std::shared_ptr<Transform> m_Transform;

public:

	GLint m_ProjectionMatrixID;
	GLint      m_ModelMatrixID;

	Renderer(std::weak_ptr<GameObject> _parent);
	~Renderer();

	/// <summary> Set the Mesh of the Renderer. </summary>
	void SetMesh(std::weak_ptr<Mesh> _mesh);

	/// <summary> Get the Mesh of the Renderer. </summary>
	std::weak_ptr<Mesh> GetMesh();

	/// <summary> Set the Material of the Renderer. </summary>
	void SetMaterial(std::weak_ptr<Material> _material);

	/// <summary> Get the Material of the Renderer. </summary>
	std::weak_ptr<Material> GetMaterial();

	/// <summary> Set the Transform of the Renderer. </summary>
	void SetTransform(std::weak_ptr<Transform> _transform);

	/// <summary> Get the Transform of the Renderer. </summary>
	std::weak_ptr<Transform> GetTransform();
};

#endif // !_RENDERER_H
