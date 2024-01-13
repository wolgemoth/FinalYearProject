#include "Renderer.h"

#include "../core/Transform.h"
#include "../ecs/Component.h"
#include "../ecs/GameObject.h"

#include "Material.h"
#include "Mesh.h"

#include <memory>

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	Renderer::Renderer(const std::shared_ptr<GameObject>& _parent) noexcept : Component(_parent),
			     m_Mesh(nullptr),
			m_Transform(nullptr) {}
	
	Renderer::~Renderer() = default;
	
	void Renderer::SetMesh(const std::shared_ptr<Mesh>& _mesh) noexcept {
		m_Mesh = _mesh;
	}
	std::shared_ptr<Mesh> Renderer::GetMesh() noexcept {
		return m_Mesh;
	}
	
	void Renderer::SetMaterial(const std::weak_ptr<Material>& _material) noexcept {
		m_Material = _material;
	}
	std::weak_ptr<Material> Renderer::GetMaterial() noexcept {
		return m_Material;
	}
	
	void Renderer::SetTransform(const std::shared_ptr<Transform>& _transform) noexcept {
		m_Transform = _transform;
	}
	std::shared_ptr<Transform> Renderer::GetTransform() noexcept {
		return m_Transform;
	}
	
} // LouiEriksson