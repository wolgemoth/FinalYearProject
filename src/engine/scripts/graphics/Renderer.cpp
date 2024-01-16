#include "Renderer.h"

#include "../core/Transform.h"
#include "../ecs/Component.h"
#include "../ecs/GameObject.h"

#include "Material.h"
#include "Mesh.h"

#include <memory>

namespace LouiEriksson::Engine::Graphics {
	
	Renderer::Renderer(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : ECS::Component(_parent) {}
	Renderer::~Renderer() = default;
	
	void Renderer::SetMesh(const std::weak_ptr<Mesh>& _mesh) noexcept {
		m_Mesh = _mesh;
	}
	const std::weak_ptr<Mesh>& Renderer::GetMesh() noexcept {
		return m_Mesh;
	}
	
	void Renderer::SetMaterial(const std::weak_ptr<Material>& _material) noexcept {
		m_Material = _material;
	}
	const std::weak_ptr<Material>& Renderer::GetMaterial() noexcept {
		return m_Material;
	}
	
	void Renderer::SetTransform(const std::weak_ptr<Transform>& _transform) noexcept {
		m_Transform = _transform;
	}
	const std::weak_ptr<Transform>& Renderer::GetTransform() noexcept {
		return m_Transform;
	}
	
} // LouiEriksson::Engine::Graphics