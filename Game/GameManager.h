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
		void ProcessExitFound();
		//void ProcessLose();

	private:
		friend class Singleton<GameManager>;
		GameManager() = default;
		bool m_ExitFound{false};



	};
}