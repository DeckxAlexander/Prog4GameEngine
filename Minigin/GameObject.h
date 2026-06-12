#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Transform.h"
#include "ObjectComponent.h"


namespace dae
{
	class Texture2D;
	class GameObject final
	{
	private:

		std::unique_ptr<Transform> m_transform;
		std::vector<std::unique_ptr<ObjectComponent>> m_Components;
		bool m_MarkedForDelete{false};

		GameObject* m_pParent;
		std::vector<GameObject*> m_Children;

	public:
		void Start();
		void Update();
		void Render() const;
		void SetPosition(float x, float y);
		void SetPosition(const glm::vec3& position);
		void SetScale(float x, float y); 

		const glm::vec3& GetWorldPosition() { return m_transform.get()->GetWorldPosition(); }
		const glm::vec3& GetScale() { return m_transform.get()->GetScale(); }
		const std::vector<GameObject*>& GetChildren() { return m_Children; }
		Transform* GetTransform() { return m_transform.get(); }

		
		void AddComponent(std::unique_ptr<ObjectComponent> component, bool enabled = true);
		void RemoveComponent(const ObjectComponent& component);
		template <typename T>
		T* GetComponentByType() const
		{
			static_assert(std::is_base_of<ObjectComponent, T>::value, "Type must derive from ObjectComponent");

			for (const auto& component : m_Components) 
			{
				if (auto compTarget = dynamic_cast<T*>(component.get()))
				{
					return compTarget;
				}
			}
			return nullptr;
		}
		template <typename T>
		bool HasComponentOfType() const 
		{
			static_assert(std::is_base_of<ObjectComponent, T>::value, "Type must derive from ObjectComponent");

			return GetComponentByType<T>() != nullptr;
		}


		//Deletion
		void MarkForDelete();
		bool IsMarkedForDelete() { return m_MarkedForDelete; }

		//Parent/Child
		void SetParent(GameObject* gameObject, bool keepWorldPosition = true);
		GameObject* GetParent() { return m_pParent; };
		bool IsDecendant(GameObject* gameObject);


		GameObject();
		GameObject(std::unique_ptr<Transform> transform);
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	};

	

}
