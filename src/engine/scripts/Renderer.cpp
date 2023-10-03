#include "stdafx.h"

#include "Renderer.h"

namespace LouiEriksson {
	
	Renderer::Renderer(std::weak_ptr<GameObject> _parent) : Component(std::move(_parent)) {
	
		     m_Mesh = std::shared_ptr<Mesh>     (nullptr);
		 m_Material = std::shared_ptr<Material> (nullptr);
		m_Transform = std::shared_ptr<Transform>(nullptr);
	
		m_ProjectionMatrixID = 0;
		     m_ModelMatrixID = 0;
	}
	
	Renderer::~Renderer() = default;
	
	void Renderer::SetMesh(const std::weak_ptr<Mesh>& _mesh) {
		m_Mesh = _mesh.lock();
	}
	std::weak_ptr<Mesh> Renderer::GetMesh() {
		return m_Mesh;
	}
	
	void Renderer::SetMaterial(const std::weak_ptr<Material>& _material) {
		
		m_Material = _material.lock();
	
		auto shader = m_Material->GetShader();
	
		m_ProjectionMatrixID = shader->GetUniform("u_Projection");
		     m_ModelMatrixID = shader->GetUniform("u_Model");
	}
	std::weak_ptr<Material> Renderer::GetMaterial() {
		return m_Material;
	}
	
	void Renderer::SetTransform(const std::weak_ptr<Transform>& _transform) {
		m_Transform = _transform.lock();
	}
	std::weak_ptr<Transform> Renderer::GetTransform() {
		return m_Transform;
	}
}