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
		Transform m_transform{};
		std::vector<std::unique_ptr<ObjectComponent>> m_Components{};
	public:
		virtual void Update();
		virtual void Render() const;


		void SetPosition(float x, float y);
		void SetScale(float x, float y);
		
		void AddComponent(std::unique_ptr<ObjectComponent> component);
		void RemoveComponent(const ObjectComponent& component);



		ObjectComponent* GetComponentByName(const std::string& name) const;
		template <typename T>
		ObjectComponent* GetComponentByType() const
		{
			static_assert(std::is_base_of<ObjectComponent, T>::value, "Type must derive from ObjectComponent");

			for (const auto& component : m_Components) 
			{
				if (typeid(*component) == typeid(T)) 
				{
					return component.get();
				}
			}

			return nullptr;
		}


		bool HasComponentOfName(const std::string& name) const;

		template <typename T>
		bool HasComponentOfType() const 
		{
			static_assert(std::is_base_of<ObjectComponent, T>::value, "Type must derive from ObjectComponent");

			return GetComponentByType<T>() != nullptr;
		}
		

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	};

	

}
