#ifndef FINALYEARPROJECT_RENDERER_HPP
#define FINALYEARPROJECT_RENDERER_HPP

#include "../ecs/GameObject.hpp"

#include "Material.hpp"
#include "Mesh.hpp"

#include <memory>
#include <typeindex>

namespace LouiEriksson::Engine {
	
	struct Transform;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Graphics {
	
	class Material;
	class Mesh;
	
} // LouiEriksson::Engine::Graphics

namespace LouiEriksson::Engine::Graphics {
	
	class Renderer final : public ECS::Component {
	
	private:
	
		std::shared_ptr<Mesh>      m_Mesh;      /**< @brief The Mesh of the Renderer. */
		std::  weak_ptr<Material>  m_Material;  /**< @brief The Material of the Renderer. */
		std::  weak_ptr<Transform> m_Transform; /**< @brief The Transform of the Renderer. */
	
		/** @brief Whether or not the Renderer casts shadows. */
		bool m_CastShadows;
		
	public:
	
		explicit Renderer(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : ECS::Component(_parent),
			m_CastShadows(true) {}
		
		/** @inheritdoc */
		[[nodiscard]] virtual std::type_index TypeID() const noexcept override { return typeid(Renderer); };
		
		/**
		 * @brief Set whether or not the Renderer should cast shadows.
		 *
		 * @param[in] _enable A boolean value indicating whether shadows should be cast or not.
		 *                Set it to true to enable shadow casting, and false to disable it.
		 */
		void Shadows(const bool& _enable) noexcept {
			m_CastShadows = _enable;
		}
		
		/**
		 * @brief Get whether or not the Renderer should cast shadows.
		 *
		 * @return A reference to a boolean value indicating whether shadows should be cast or not.
		 *         Returns true if shadows should be cast, and false otherwise.
		 */
		[[nodiscard]] constexpr const bool& Shadows() const noexcept {
			return m_CastShadows;
		}
		
		/**
		 * @brief Set the Mesh of the Renderer.
		 *
		 * This function sets the mesh of the renderer to the provided mesh.
		 *
		 * @param[in] _mesh A weak pointer to the Mesh object to set.
		 */
		void SetMesh(const std::weak_ptr<Mesh>& _mesh) noexcept {
			
			if (const auto m = _mesh.lock()) {
				m_Mesh = m;
			}
		}
		
		/**
		 * @brief Get the Mesh of the Renderer.
		 *
		 * This function returns a weak pointer to the Mesh object associated with the Renderer.
		 *
		 * @return A weak pointer to the Mesh object.
		 */
		std::weak_ptr<Mesh> GetMesh() noexcept {
			return m_Mesh;
		}
		
		/**
		 * @brief Set the Material of the Renderer.
		 *
		 * This function sets the Material of the Renderer to the provided Material.
		 *
		 * @param[in] _material A std::weak_ptr to the Material object to set.
		 */
		void SetMaterial(const std::weak_ptr<Material>& _material) noexcept {
			m_Material = _material;
		}
		
		/**
		 * @brief Get the Material of the Renderer.
		 *
		 * This function returns a weak pointer to the Material object associated with the Renderer.
		 *
		 * @return A weak pointer to the Material object.
		 */
		constexpr const std::weak_ptr<Material>& GetMaterial() noexcept {
			return m_Material;
		}
		
		/**
		 * @brief Set the Transform of the Renderer.
		 *
		 * This function sets the Transform of the Renderer to the provided Transform.
		 *
		 * @param[in] _transform A std::weak_ptr to the Transform object to set.
		 */
		void SetTransform(const std::weak_ptr<Transform>& _transform) noexcept {
			m_Transform = _transform;
		}
		
		/**
		 * @brief Get the Transform of the Renderer.
		 *
		 * @return const std::weak_ptr<Transform>& The weak pointer to the Transform.
		 */
		constexpr const std::weak_ptr<Transform>& GetTransform() noexcept {
			return m_Transform;
		}
		
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_RENDERER_HPP