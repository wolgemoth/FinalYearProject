#include "stdafx.h"

#include "Ball.h"

namespace LouiEriksson {
	
	Ball::Ball(std::weak_ptr<GameObject> _parent) : Script(std::move(_parent)) {
		m_Material = std::shared_ptr<Material>(nullptr);
	}
	
	Ball::~Ball() = default;
	
	void Ball::Begin() {
	
		auto parent = GetGameObject().lock();
	
		auto scene = parent->GetScene().lock();
	
		auto mesh = Mesh::Load("models/sphere/sphere.obj");
	
		// Compile shader.
		auto diffuse = scene->Attach(Shader::Create({
			{ "shaders/diffuse/diffuse.vert", GL_VERTEX_SHADER   },
			{ "shaders/diffuse/diffuse.frag", GL_FRAGMENT_SHADER }
		}));
		diffuse->BindAttribute(0, "a_Position");
		diffuse->BindAttribute(1, "a_TexCoord");
	
		// Create material from shader.
		m_Material = Material::Create(diffuse);
		m_Material->Texture_ID(mesh->Texture_ID());
	
		// Get or add component.
		auto transform = parent->GetComponent<Transform>().lock();
		if (transform == nullptr) {
			transform = parent->AddComponent<Transform>().lock();
		}
	
		// Get radius.
		float r = glm::length(transform->m_Scale) / 2.0f;
	
		// Get or add renderer.
		auto renderer = scene->Attach(parent->AddComponent<Renderer>().lock());
		if (renderer == nullptr) {
			renderer = parent->AddComponent<Renderer>().lock();
		}
	
		renderer->SetMesh(mesh);
		renderer->SetMaterial(m_Material);
		renderer->SetTransform(transform);
	
		// Get or add collider.
		auto collider = scene->Attach<Collider>(parent->AddComponent<Collider>().lock());
		if (collider == nullptr) {
			collider = parent->AddComponent<Collider>().lock();
		}
	
		collider->SetTransform(transform);
		collider->SetType(Collider::Type::Sphere);
		collider->Radius(r);
	
		// Get or add rigidbody.
		auto rigidbody = scene->Attach(parent->AddComponent<Rigidbody>().lock());
		if (rigidbody == nullptr) {
			rigidbody = parent->AddComponent<Rigidbody>().lock();
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