#include "TextComponent.h"
#include "EventManagers.h"
#include "HealthComponent.h"
namespace dae
{
	class HealthObserver : public Observer
	{
	public:
		HealthObserver(GameObject* pOwner) : Observer(pOwner)
		{
		}

		~HealthObserver() = default;

		virtual void OnNotify(const Event& event) override
		{
			switch (event.event)
			{
			case EventType::PlayerDead:

				std::stringstream ss;
				ss << std::to_string(dynamic_cast<HealthComponent*>(event.sender)->GetHealth()) << " Health";
				if (auto textComp = m_pOwner->GetComponentByType<TextComponent>()) textComp->SetText(ss.str());
				break;
			}
		}

	};




}