#include "stdafx.h"

#include "Renderer.h"

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	Renderer::Renderer(const std::shared_ptr<GameObject>& _parent) : Component(_parent) {
	
		     m_Mesh = std::shared_ptr<Mesh>     (nullptr);
		 m_Material = std::shared_ptr<Material> (nullptr);
		m_Transform = std::shared_ptr<Transform>(nullptr);
	}
	
	Renderer::~Renderer() = default;
	
	void Renderer::SetMesh(const std::shared_ptr<Mesh>& _mesh) {
		m_Mesh = _mesh;
	}
	std::shared_ptr<Mesh> Renderer::GetMesh() {
		return m_Mesh;
	}
	
	void Renderer::SetMaterial(const std::weak_ptr<Material>& _material) {
		m_Material = _material;
	}
	std::weak_ptr<Material> Renderer::GetMaterial() {
		return m_Material;
	}
	
	void Renderer::SetTransform(const std::shared_ptr<Transform>& _transform) {
		m_Transform = _transform;
	}
	std::shared_ptr<Transform> Renderer::GetTransform() {
		return m_Transform;
	}
}