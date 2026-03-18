#include "TextComponent.h"
#include "EventManagers.h"
#if USE_STEAMWORKS
#pragma warning (push)
#pragma warning (disable:4996)
#include <steam_api.h>
#pragma warning (pop)
#endif

namespace dae
{
	class SteamObserver : public Observer
	{
	public:
		SteamObserver(GameObject* pOwner) : Observer(pOwner)
		{
		}

		~SteamObserver() = default;

		virtual void OnNotify(const Event& event) override
		{
			switch (event.event)
			{
			case EventType::Win:

				UnlockAchievement("ACH_WIN_ONE_GAME");
			default:
				// do nothing 
				break;
			}
		}

		void UnlockAchievement(const char* achievementID)
		{
#if USE_STEAMWORKS
			if (SteamUserStats() && SteamUserStats()->SetAchievement(achievementID) && !m_HasWon)
			{
				SteamUserStats()->StoreStats(); // make sure it's saved
				printf("Achievement unlocked: %s\n", achievementID);
				m_HasWon = true;
			}
			#else
			achievementID;
			m_HasWon = true;
#endif
		}
	private:
		bool m_HasWon{ false }; //TEMP
	};
}