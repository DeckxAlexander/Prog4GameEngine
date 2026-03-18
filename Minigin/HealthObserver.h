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
			std::stringstream ss;
			switch (event.event)
			{
			case EventType::PlayerDead:

				ss << std::to_string(dynamic_cast<HealthComponent*>(event.sender)->GetHealth()) << " Health";
				if (auto textComp = m_pOwner->GetComponentByType<TextComponent>()) textComp->SetText(ss.str());
				break;
			default:
				// do nothing for other events (or log)
				break;
			}
		}

	};




}