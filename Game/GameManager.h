#include "Singleton.h"
#include <vector>

namespace dae
{
	class GameObject;
	class GameManager final : public Singleton<GameManager>
	{
	public:
		void CheckGameState();
		void ProcessWin();
		void ProcessGameOver();
		void ResetGame();
		void SetPlayerAmount(int amount);
		void SetVersus(bool value);

		void CheckPlayerDeath();
		//void ProcessLose();

	private:
		friend class Singleton<GameManager>;
		GameManager() = default;
		bool m_ExitFound{false};
		int m_CurrentLevel{ 1 };
		int m_IsVersus{ false };
		int m_PlayerAmount{};

	};
}