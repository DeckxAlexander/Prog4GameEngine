#pragma once
#include "ObjectComponent.h"

namespace dae
{

	class DestructibleComponent final : public ObjectComponent
	{

	public:
		virtual void Update() override {};
		virtual void Render() const override {};
		void DestroyObject();

		DestructibleComponent() : ObjectComponent()
		{
		}
		~DestructibleComponent() {}
		DestructibleComponent(const DestructibleComponent& other) = delete;
		DestructibleComponent(DestructibleComponent&& other) = delete;
		DestructibleComponent& operator=(const DestructibleComponent& other) = delete;
		DestructibleComponent& operator=(DestructibleComponent&& other) = delete;
	};
}
