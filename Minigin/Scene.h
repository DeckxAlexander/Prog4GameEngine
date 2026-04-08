#pragma once
#include <memory>
#include <string>
#include <vector>
#include "GameObject.h"

namespace dae
{
	class Scene final
	{
	public:
		void Add(std::unique_ptr<GameObject> object);
		void Remove(GameObject& object);
		void Cleanup();
		void RemoveAll();

		void Update();
		void Render() const;

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		template <typename T>
		std::vector<GameObject*> GetAllObjectsByComponent() 
		{

			std::vector<GameObject*> result{};

			static_assert(std::is_base_of<ObjectComponent, T>::value, "Type must derive from ObjectComponent");
			for (const auto& object : m_objects)
			{
				if (object.get()->GetComponentByType<T>() != nullptr)
				{
					result.push_back(object.get());
				}
			}

			return result;
		}


	private:
		friend class SceneManager;
		explicit Scene() = default;
		std::vector < std::unique_ptr<GameObject>> m_objects{};
	};

}
