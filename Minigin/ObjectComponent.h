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



		GameObject* GetOwner() const;
		ObjectComponent(GameObject* pOwner);
		virtual ~ObjectComponent() = default;
		ObjectComponent(const ObjectComponent& other) = delete;
		ObjectComponent(ObjectComponent&& other) = delete;
		ObjectComponent& operator=(const ObjectComponent& other) = delete;
		ObjectComponent& operator=(ObjectComponent&& other) = delete;
	};


	class PlayerComponent : public ObjectComponent
	{
	private:
		int m_PlayerIndex;
	public:
		virtual void Update() override { m_PlayerIndex; };
		virtual void Render() const override {};
		PlayerComponent(GameObject* pOwner, int index) : ObjectComponent(pOwner), m_PlayerIndex{index}
		{
		}
		virtual ~PlayerComponent() = default;
		PlayerComponent(const PlayerComponent& other) = delete;
		PlayerComponent(PlayerComponent&& other) = delete;
		PlayerComponent& operator=(const PlayerComponent& other) = delete;
		PlayerComponent& operator=(PlayerComponent&& other) = delete;
	};
}