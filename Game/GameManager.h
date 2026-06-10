#include "Singleton.h"
#include <vector>
#include "EventManagers.h"


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
		void AddScore(int amount);

		int GetScore() { return m_Score; }
		void CheckPlayerDeath();
		Subject* GetSubject() { return m_Subject.get(); }

	private:
		friend class Singleton<GameManager>;
		GameManager();
		bool m_ExitFound{false};
		int m_CurrentLevel{ 1 };
		int m_IsVersus{ false };
		int m_PlayerAmount{};
		int m_Score{};
		std::unique_ptr<Subject> m_Subject;
	};
}