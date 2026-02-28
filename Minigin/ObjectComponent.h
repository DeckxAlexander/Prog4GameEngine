#pragma once
#include <string>
#include <memory>
#include "Transform.h"

namespace dae
{
	class GameObject;


	class ObjectComponent //Abstract class
	{


	protected:
		//These variables are kept incase object has Render capabilities
		GameObject* m_pOwner;
	public:
		friend class GameObject;
		virtual void Update() = 0;
		virtual void Render() const = 0;



		GameObject* GetOwner();
		ObjectComponent(GameObject* pOwner);
		virtual ~ObjectComponent() = default;
		ObjectComponent(const ObjectComponent& other) = delete;
		ObjectComponent(ObjectComponent&& other) = delete;
		ObjectComponent& operator=(const ObjectComponent& other) = delete;
		ObjectComponent& operator=(ObjectComponent&& other) = delete;
	};
}