#pragma once
#include "ObjectComponent.h"
#include "EventManagers.h"



namespace dae
{
	class CollisionComponent;
	class ExitComponent final : public ObjectComponent, public IObserver
	{
	private:
		std::vector<CollisionComponent*> m_PlayerColliders{};
		CollisionComponent* m_Collider{ nullptr };
		bool m_CheckActive{ false };
	public:
		virtual void Start() override;
		virtual void Update() override;
		virtual void Render() const override {}
		void InitializePlayers();


		ExitComponent();
		~ExitComponent();
		ExitComponent(const ExitComponent& other) = delete;
		ExitComponent(ExitComponent&& other) = delete;
		ExitComponent& operator=(const ExitComponent& other) = delete;
		ExitComponent& operator=(ExitComponent&& other) = delete;


		virtual void OnSubjectDestroyed(Subject* subject) { subject; }
		virtual void OnNotify(const Event& event);
	};
}