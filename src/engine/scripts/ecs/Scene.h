#ifndef FINALYEARPROJECT_SCENE_H
#define FINALYEARPROJECT_SCENE_H

#include "../core/utils/Hashmap.h"
#include "../graphics/Camera.h"

#include <filesystem>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <vector>

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	class Application;
	
} // LouiEriksson

namespace LouiEriksson::ECS {
	
	class Component;
	class GameObject;
	
	class Scene : public std::enable_shared_from_this<Scene> {
	
		friend LouiEriksson::Application;
		friend GameObject;
	
	private:
	
		/// <summary> Render the Scene. </summary>
		void Draw(const LouiEriksson::Graphics::Camera::RenderFlags& _flags);
		
	protected:
	
		/// <summary> Entities within the Scene. </summary>
		std::vector<std::shared_ptr<GameObject>> m_Entities;
		
		/// <summary> Components (linked to entities) within the Scene. </summary>
		Hashmap<std::type_index, std::vector<std::weak_ptr<Component>>> m_Components;
	
		/// <summary> Called at the beginning of the first frame. </summary>
		virtual void Begin();
	
		/// <summary> Called every frame. </summary>
		virtual void Tick(const Graphics::Camera::RenderFlags& _flags);
	
		/// <summary> Called every physics update. </summary>
		virtual void FixedTick();
	
	public:
	
		Scene() = default;
		virtual ~Scene();
	
		/// <summary> Get the components within the Scene. </summary>
		const Hashmap<std::type_index, std::vector<std::weak_ptr<Component>>>& Components() noexcept;
	
		/// <summary> Save the Scene in xml format at a given path. </summary>
		void Save(const std::filesystem::path& _path);
	
		/// <summary> TryLoad the Scene from xml format at a given path. </summary>
		static std::shared_ptr<Scene> Load(const std::filesystem::path& _path);
	
		/// <summary> Attach an instance of a type to the Scene. </summary>
		template<class T>
		std::shared_ptr<T> Attach(std::shared_ptr<T> _entity) {
	
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
	
			std::vector<std::weak_ptr<Component>> category;
			m_Components.Get(typeid(T), category);
	
			category.emplace_back(std::dynamic_pointer_cast<T>(_entity));
			m_Components.Assign(typeid(T), category);
	
			return _entity;
		}
		
		/// <summary> Detach an instance of a type from the Scene. </summary>
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
	
} // LouiEriksson::ECS

#endif //FINALYEARPROJECT_SCENE_H