#ifndef FINALYEARPROJECT_SCENE_H
#define FINALYEARPROJECT_SCENE_H

#include "../core/utils/Hashmap.h"
#include "../graphics/Camera.h"

#include <filesystem>
#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>
#include <vector>

namespace LouiEriksson::Engine {
	
	class Application;
	class Script;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::ECS {
	
	class Component;
	class GameObject;
	
	/**
	 * @class Scene
	 * @brief Represents a scene in the application.
	 *
	 * The Scene class is responsible for managing entities and components within a scene.
	 */
	class Scene final : public std::enable_shared_from_this<Scene> {
	
		friend LouiEriksson::Engine::Application;
		friend GameObject;
	
	private:
		
		/**
		 * @fn void Scene::Draw(const LouiEriksson::Engine::Graphics::Camera::RenderFlags& _flags)
		 * @brief Render the Scene.
		 *
		 * @param[in] _flags The render flags specifying what actions to take during the render process.
		 */
		void Draw(const LouiEriksson::Engine::Graphics::Camera::RenderFlags& _flags);
		
	protected:
	
		/** @brief Entities within the Scene. */
		std::vector<std::shared_ptr<GameObject>> m_Entities;
		
		/** @brief Components (linked to entities) within the Scene. */
		Hashmap<std::type_index, std::vector<std::weak_ptr<Component>>> m_Components;
		
		/** @brief Called at the beginning of the first frame. */
		virtual void Begin();
	
		/**
		 * @brief Called every frame.
		 * @param[in] _flags The render flags specifying what actions to take during the render process.
		 */
		virtual void Tick(const Graphics::Camera::RenderFlags& _flags);
	
		/** @brief Called every physics update. */
		virtual void FixedTick();
		
	public:
	
		Scene() = default;
		virtual ~Scene();
		
		/**
		 * @brief Get the components within the Scene.
		 *
		 * This function returns a reference to a hashmap containing the components within the scene.
		 *
		 * @return const Hashmap<std::type_index, std::vector<std::weak_ptr<Component>>>& - a reference to the hashmap containing the components
		 */
		const Hashmap<std::type_index, std::vector<std::weak_ptr<Component>>>& Components() noexcept;
		
		/**
		 * @fn void Scene::Save(const path &_path)
		 * @brief Save the Scene in XML format at a given path.
		 *
		 * @param[in] _path - The path to save the Scene in xml format.
		 */
		void Save(const std::filesystem::path& _path);
		
		/**
		 * @brief Loads a scene from a file.
		 *
		 * @param[in] _path - The path to the scene file.
		 * @param[in] _initialisers - A Hashmap of scripts types and their initialisers.
		 * @return std::shared_ptr<Scene> - A shared pointer to the loaded scene.
		 */
		static std::shared_ptr<Scene> Load(const std::filesystem::path& _path, const Hashmap<std::string, std::shared_ptr<Script> (*)(const std::weak_ptr<ECS::GameObject>& parent)>& _initialisers);
		
		/**
		 * @brief Attach an instance of a type to the Scene.
		 *
		 * This function attaches an instance of a type to the Scene. The provided type must derive from the "Component" class.
		 *
		 * @tparam T The type of the instance to be attached.
		 * @param _entity A shared pointer to the instance to be attached.
		 * @return A shared pointer to the attached instance.
		 *
		 * @relates Scene
		 */
		template<class T>
		std::shared_ptr<T> Attach(std::shared_ptr<T> _entity) {
	
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
	
			std::vector<std::weak_ptr<Component>> category;
			m_Components.Get(typeid(T), category);
	
			category.emplace_back(std::dynamic_pointer_cast<T>(_entity));
			m_Components.Assign(typeid(T), category);
	
			return _entity;
		}
		
		/**
		 * @fn void Scene::Detach(const std::weak_ptr<T>& _entity)
		 * @brief Detach an instance of a type from the Scene.
		 *
		 * This function detaches an instance of a type from the Scene. The provided type must derive from the "Component" class.
		 *
		 * @tparam T The type of the instance to be detached.
		 * @param _entity A weak pointer to the instance to be detached.
		 *
		 * @remark If the provided weak pointer is expired or if the instance is not found within the Scene, no action is taken.
		 *
		 * @relates Scene
		 */
		template<typename T>
		void Detach(const std::weak_ptr<T>& _entity) {
			
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
	
			if (const auto e = _entity.lock()) {
				
				std::vector<std::weak_ptr<Component>> category;
				if (m_Components.Get(typeid(T), category)) {
		
					for (auto itr = category.begin(); itr < category.end(); ++itr) {
		
						if (std::dynamic_pointer_cast<T>(*itr).get() == e.get()) {
							category.erase(itr);
							break;
						}
					}
				}
			}
		}
		
	};
	
	template<>
	inline std::shared_ptr<GameObject> Scene::Attach(std::shared_ptr<GameObject> _entity) {
		
		m_Entities.emplace_back(_entity);
		
		return _entity;
	}
	
	template<>
	inline void Scene::Detach(const std::weak_ptr<GameObject>& _entity) {
		
		if (const auto e = _entity.lock()) {
			
			for (auto itr = m_Entities.begin(); itr < m_Entities.end(); ++itr) {
				
				if ((*itr).get() == e.get()) {
					m_Entities.erase(itr);
					break;
				}
			}
		}
	}
	
	template<>
	inline void Scene::Detach(const std::weak_ptr<Component>& _entity) {
		
		if (const auto e = _entity.lock()) {
			
			for (auto& bucket : m_Components.Values()) {
				
				for (auto itr = bucket.begin(); itr < bucket.end(); ++itr) {
					
					auto item = *itr;
					
					if (const auto locked = item.lock()) {
						
						if (locked.get() == e.get()) {
							bucket.erase(itr);
							break;
						}
					}
					else {
						bucket.erase(itr);
					}
				}
			}
		}
	}
	
} // LouiEriksson::Engine::ECS

#endif //FINALYEARPROJECT_SCENE_H