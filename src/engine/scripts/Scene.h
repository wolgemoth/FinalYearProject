#pragma once

#ifndef _SCENE_H
#define _SCENE_H

#include "Components.h"

#include "Serialisation.h"

//#include <Windows.h>

class Application;
class GameObject;
class Script;

class Scene : public std::enable_shared_from_this<Scene> {

friend class Application;

private:

	/// <summary> Render the Scene. </summary>
	void Draw();
	
protected:

	/// <summary> Entities within the Scene. </summary>
	Hashmap<std::type_index, std::vector<std::any>> m_Entities;

	/// <summary> Called at the beginning of the first frame. </summary>
	virtual void Begin();

	/// <summary> Called every frame. </summary>
	virtual void Tick();

	/// <summary> Called every physics update. </summary>
	virtual void FixedTick();

public:

	Scene() = default;

	virtual ~Scene();

	/// <summary> Get the entities within the Scene. </summary>
	Hashmap<std::type_index, std::vector<std::any>> Entities();

	/// <summary> Save the Scene in xml format at a given path. </summary>
	void Save(std::filesystem::path _path);

	/// <summary> Load the Scene from xml format at a given path. </summary>
	static std::shared_ptr<Scene> Load(std::filesystem::path _path);

	/// <summary> Attach an instance of a type to the Scene. </summary>
	template<class T>
	std::shared_ptr<T> Attach(std::shared_ptr<T> _entity) {

		std::vector<std::any> category;
		m_Entities.Get(typeid(T), category);

		category.push_back(std::make_any<std::shared_ptr<T>>(_entity));
		m_Entities.Assign(typeid(T), category);

		return _entity;
	}

	/// <summary> Detach an instance of a type from the Scene. </summary>
	template<typename T>
	void Detach(std::weak_ptr<T> _entity) {
		
		std::vector<std::any> category;
		if (m_Entities.Get(typeid(T), category)) {

			T* ptr1 = _entity.get();

			for (auto itr = category.begin(); itr < category.end(); itr++) {

				T* ptr2 = std::any_cast<std::shared_ptr<T>>(*itr).get();

				if (ptr1 == ptr2) {
					category.erase(itr);
					break;
				}
			}
		}
	}
};

#endif // !_SCENE_H
