#ifndef FINALYEARPROJECT_RENDERER_H
#define FINALYEARPROJECT_RENDERER_H

#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

namespace LouiEriksson {
	
	class Renderer : public Component {
	
	private:
	
		/// <summary> Mesh of the Renderer. </summary>
		std::shared_ptr<Mesh> m_Mesh;
	
		/// <summary> Material of the Renderer. </summary>
		std::weak_ptr<Material> m_Material;
	
		/// <summary> Transform of the Renderer. </summary>
		std::shared_ptr<Transform> m_Transform;
	
	public:
	
		explicit Renderer(const std::shared_ptr<GameObject>& _parent);
		~Renderer() override;
	
		/// <summary> Set the Mesh of the Renderer. </summary>
		void SetMesh(const std::shared_ptr<Mesh>& _mesh);
	
		/// <summary> Get the Mesh of the Renderer. </summary>
		std::shared_ptr<Mesh> GetMesh();
	
		/// <summary> Set the Material of the Renderer. </summary>
		void SetMaterial(const std::weak_ptr<Material>& _material);
	
		/// <summary> Get the Material of the Renderer. </summary>
		std::weak_ptr<Material> GetMaterial();
	
		/// <summary> Set the Transform of the Renderer. </summary>
		void SetTransform(const std::shared_ptr<Transform>& _transform);
	
		/// <summary> Get the Transform of the Renderer. </summary>
		std::shared_ptr<Transform> GetTransform();
	};
}

#endif //FINALYEARPROJECT_RENDERER_H