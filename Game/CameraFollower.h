#pragma once
#include "ObjectComponent.h"
#include "EventManagers.h"

namespace dae {
    class CameraFollowerComponent final : public ObjectComponent, public IObserver
	{
	private:
		GameObject* m_Player{};
		
		void InitializePlayer();
	public:
		virtual void Start() override;
		virtual void Update() override;
		virtual void Render() const override {}

		CameraFollowerComponent(GameObject* pOwner);
		~CameraFollowerComponent();
		CameraFollowerComponent(const CameraFollowerComponent& other) = delete;
		CameraFollowerComponent(CameraFollowerComponent&& other) = delete;
		CameraFollowerComponent& operator=(const CameraFollowerComponent& other) = delete;
		CameraFollowerComponent& operator=(CameraFollowerComponent&& other) = delete;

		virtual void OnNotify(const Event& event);
		virtual void OnSubjectDestroyed(Subject* subject);

    };
}
