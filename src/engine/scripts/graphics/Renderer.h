#ifndef FINALYEARPROJECT_RENDERER_H
#define FINALYEARPROJECT_RENDERER_H

#include "../ecs/Component.h"

#include <memory>
#include <typeindex>

namespace LouiEriksson::Engine {
	
	class Transform;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Graphics {
	
	class Material;
	class Mesh;
	
} // LouiEriksson::Engine::Graphics

namespace LouiEriksson::Engine::Graphics {
	
	class Renderer final : public ECS::Component {
	
	private:
	
		/// <summary> Mesh of the Renderer. </summary>
		std::weak_ptr<Mesh> m_Mesh;
	
		/// <summary> Material of the Renderer. </summary>
		std::weak_ptr<Material> m_Material;
	
		/// <summary> Transform of the Renderer. </summary>
		std::weak_ptr<Transform> m_Transform;
	
		/// <summary> Whether or not the Renderer is a shadow caster. </summary>
		bool m_CastShadows;
		
	public:
	
		explicit Renderer(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		~Renderer() override;
	
		[[nodiscard]] const std::type_index TypeID() const noexcept override { return typeid(Renderer); };
		
		/// <summary> Set whether or not the Renderer should cast shadows. </summary>
		void Shadows(const bool& _enable) noexcept;
	
		/// <summary> Set whether or not the Renderer should cast shadows. </summary>
		[[nodiscard]] const bool& Shadows() const noexcept;
		
		/// <summary> Set the Mesh of the Renderer. </summary>
		void SetMesh(const std::weak_ptr<Mesh>& _mesh) noexcept;
	
		/// <summary> Get the Mesh of the Renderer. </summary>
		const std::weak_ptr<Mesh>& GetMesh() noexcept;
	
		/// <summary> Set the Material of the Renderer. </summary>
		void SetMaterial(const std::weak_ptr<Material>& _material) noexcept;
	
		/// <summary> Get the Material of the Renderer. </summary>
		const std::weak_ptr<Material>& GetMaterial() noexcept;
	
		/// <summary> Set the Transform of the Renderer. </summary>
		void SetTransform(const std::weak_ptr<Transform>& _transform) noexcept;
	
		/// <summary> Get the Transform of the Renderer. </summary>
		const std::weak_ptr<Transform>& GetTransform() noexcept;
		
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_RENDERER_H