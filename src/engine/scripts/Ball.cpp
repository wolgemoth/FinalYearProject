#include "stdafx.h"

#include "Ball.h"

namespace LouiEriksson {
	
	Ball::Ball(const std::shared_ptr<GameObject>& _parent) : Script(_parent) {
		
		m_Mesh     = std::shared_ptr<Mesh>    (nullptr);
		m_Material = std::shared_ptr<Material>(nullptr);
	}
	
	Ball::~Ball() = default;
	
	void Ball::Begin() {
	
		auto scene = Parent()->GetScene();
	
		// Load mesh.
		if (m_Mesh == nullptr) {
			m_Mesh = Mesh::Load("models/sphere/sphere.obj");
		}
		
		if (m_Material == nullptr) {
			
			// Create material from shader.
			m_Material = Material::Create(Shader::m_Cache.Return("surface"));
			m_Material->Texture_ID(m_Mesh->Texture_ID());
		}
	
		// Get or add component.
		auto transform = Parent()->GetComponent<Transform>();
		if (transform == nullptr) {
			transform = Parent()->AddComponent<Transform>();
		}
	
		// Get radius.
		float r = glm::length(transform->m_Scale) / 2.0f;
	
		// Get or add renderer.
		auto renderer = scene->Attach(Parent()->AddComponent<Renderer>());
		if (renderer == nullptr) {
			renderer = Parent()->AddComponent<Renderer>();
		}
	
		renderer->SetMesh(m_Mesh);
		renderer->SetMaterial(m_Material);
		renderer->SetTransform(transform);
	
		// Get or add collider.
		auto collider = scene->Attach<Collider>(Parent()->AddComponent<Collider>());
		if (collider == nullptr) {
			collider = Parent()->AddComponent<Collider>();
		}
	
		collider->SetTransform(transform);
		collider->SetType(Collider::Type::Sphere);
		collider->Radius(r);
	
		// Get or add rigidbody.
		auto rigidbody = scene->Attach(Parent()->AddComponent<Rigidbody>());
		if (rigidbody == nullptr) {
			rigidbody = Parent()->AddComponent<Rigidbody>();
		}
		
		rigidbody->SetTransform(transform);
		rigidbody->SetCollider(collider);
		rigidbody->Mass(4.0f * glm::pi<float>() * (r * r)); // Equation for area of sphere. Courtesy of :https://www.omnicalculator.com/math/area-of-sphere
		rigidbody->Drag(0.005f); // Courtesy of: https://www.engineeringtoolbox.com/drag-coefficient-d_627.html
	
		collider->SetRigidbody(rigidbody);
	}
	
	void Ball::Tick() {
	
	}
}