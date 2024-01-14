#ifndef FINALYEARPROJECT_RENDERER_H
#define FINALYEARPROJECT_RENDERER_H

#include "../ecs/Component.h"

#include <memory>

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	class Transform;
	
} // LouiEriksson

namespace LouiEriksson::Graphics {
	
	class Material;
	class Mesh;
	
} // LouiEriksson::Graphics

namespace LouiEriksson::Graphics {
	
	class Renderer : public ECS::Component {
	
	private:
	
		/// <summary> Mesh of the Renderer. </summary>
		std::weak_ptr<Mesh> m_Mesh;
	
		/// <summary> Material of the Renderer. </summary>
		std::weak_ptr<Material> m_Material;
	
		/// <summary> Transform of the Renderer. </summary>
		std::weak_ptr<Transform> m_Transform;
	
	public:
	
		explicit Renderer(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		~Renderer() override;
	
		/// <summary> Set the Mesh of the Renderer. </summary>
		void SetMesh(const std::weak_ptr<Mesh>& _mesh) noexcept;
	
		/// <summary> Get the Mesh of the Renderer. </summary>
		const std::weak_ptr<Mesh> GetMesh() noexcept;
	
		/// <summary> Set the Material of the Renderer. </summary>
		void SetMaterial(const std::weak_ptr<Material>& _material) noexcept;
	
		/// <summary> Get the Material of the Renderer. </summary>
		const std::weak_ptr<Material> GetMaterial() noexcept;
	
		/// <summary> Set the Transform of the Renderer. </summary>
		void SetTransform(const std::weak_ptr<Transform>& _transform) noexcept;
	
		/// <summary> Get the Transform of the Renderer. </summary>
		const std::weak_ptr<Transform> GetTransform() noexcept;
		
	};
	
} // LouiEriksson::Graphics

#endif //FINALYEARPROJECT_RENDERER_H