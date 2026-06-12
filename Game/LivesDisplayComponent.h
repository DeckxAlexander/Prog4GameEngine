#pragma once
#include "ObjectComponent.h"
#include "EventManagers.h"

namespace dae
{
	class LivesDisplayComponent : public ObjectComponent, public IObserver
	{
	public:
		virtual void Start() override;
		virtual void Update() override {}
		void InitializePlayers();
		void UpdateDisplay();

		LivesDisplayComponent();
		~LivesDisplayComponent();
		LivesDisplayComponent(const LivesDisplayComponent& other) = delete;
		LivesDisplayComponent(LivesDisplayComponent&& other) = delete;
		LivesDisplayComponent& operator=(const LivesDisplayComponent& other) = delete;
		LivesDisplayComponent& operator=(LivesDisplayComponent&& other) = delete;

		virtual void OnNotify(const Event& event) override;
		virtual void OnSubjectDestroyed(Subject*) override {}
	private:
		std::vector<GameObject*> m_Players{};
	};


}
