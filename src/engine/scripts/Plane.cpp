#include "stdafx.h"

#include "Plane.h"

namespace LouiEriksson {
	
	Plane::Plane(std::weak_ptr<GameObject> _parent) : Script(std::move(_parent)) {
		m_Material = std::shared_ptr<Material>(nullptr);
	}
	
	Plane::~Plane() = default;
	
	void Plane::Begin() {
	
		auto parent = GetGameObject().lock();
	
		auto scene = parent->GetScene().lock();
	
		// Compile shader.
		auto diffuse = scene->Attach(Shader::Create({
				{ "shaders/diffuse/diffuse.vert", GL_VERTEX_SHADER   },
				{ "shaders/diffuse/diffuse.frag", GL_FRAGMENT_SHADER }
		}));
		diffuse->BindAttribute(0, "a_Position");
		diffuse->BindAttribute(1, "a_TexCoord");
	
		// Load mesh.
		auto mesh = Mesh::Load("models/woodfloor/woodfloor.obj");
	
		// Create material from shader.
		m_Material = Material::Create(diffuse);
		m_Material->Texture_ID(mesh->Texture_ID());
	
		// Get or add Transform.
		auto transform = parent->GetComponent<Transform>().lock();
		if (transform == nullptr) {
			transform = parent->AddComponent<Transform>().lock();
		}
	
		// Get or add Renderer.
		auto renderer = scene->Attach(parent->AddComponent<Renderer>().lock());
		if (renderer == nullptr) {
			renderer = parent->AddComponent<Renderer>().lock();
		}
	
		renderer->SetMesh(mesh);
		renderer->SetMaterial(m_Material);
		renderer->SetTransform(transform);
	
		// Get or add Collider.
		auto collider = scene->Attach<Collider>(parent->AddComponent<Collider>().lock());
		if (collider == nullptr) {
			collider = parent->AddComponent<Collider>().lock();
		}
		
		collider->SetTransform(transform);
		collider->SetType(Collider::Type::Plane);
	}
	
	void Plane::Tick() {
	
	}
}