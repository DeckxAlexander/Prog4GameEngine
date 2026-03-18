#pragma once
#include "TextComponent.h"
#include "EventManagers.h"
#include "HealthComponent.h"
namespace dae
{
	class HealthDisplayComponent final : public ObjectComponent
	{
	public:
		void Update() override
		{
		}

		void Render() const override
		{
		}

		void SetHealth(int health)
		{
			if (auto textComp = m_pOwner->GetComponentByType<TextComponent>()) textComp->SetText(std::to_string(health));
		}

		HealthDisplayComponent(GameObject* pOwner) : ObjectComponent(pOwner)
		{
		}
		~HealthDisplayComponent() = default;
		HealthDisplayComponent(const HealthDisplayComponent& other) = delete;
		HealthDisplayComponent(HealthDisplayComponent&& other) = delete;
		HealthDisplayComponent& operator=(const HealthDisplayComponent& other) = delete;
		HealthDisplayComponent& operator=(HealthDisplayComponent&& other) = delete;

	};

	class HealthObserver : public Observer
	{
	public:
		HealthObserver(HealthDisplayComponent* display) : Observer(), m_display{ display }
		{
		}

		~HealthObserver() = default;

		virtual void OnNotify(const Event& event) override
		{
			switch (event.event)
			{
			case EventType::PlayerDead:
				m_display->SetHealth(dynamic_cast<HealthComponent*>(event.sender)->GetHealth());
				break;
			}
		}

	private:
		HealthDisplayComponent* m_display;
	};




}