#include "ObjectComponent.h"
#include "EventManagers.h"

namespace dae
{
	class ScoreDisplayComponent : public ObjectComponent, public IObserver
	{
	public:
		virtual void Start() override;
		virtual void Update() override {}

		ScoreDisplayComponent();
		~ScoreDisplayComponent();
		ScoreDisplayComponent(const ScoreDisplayComponent& other) = delete;
		ScoreDisplayComponent(ScoreDisplayComponent&& other) = delete;
		ScoreDisplayComponent& operator=(const ScoreDisplayComponent& other) = delete;
		ScoreDisplayComponent& operator=(ScoreDisplayComponent&& other) = delete;

		virtual void OnNotify(const Event& event) override;
		virtual void OnSubjectDestroyed(Subject* subject) override;
	private:
		bool m_SubjectDestroyed{ false };
	};


}
