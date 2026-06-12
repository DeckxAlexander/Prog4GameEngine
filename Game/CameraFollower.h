#include "ObjectComponent.h"
#include "EventManagers.h"

namespace dae {
    class CameraFollowerComponent final : public ObjectComponent, public IObserver
	{
	private:
		GameObject* m_Player{};
		float m_MapX;
		float m_MapY;
		void InitializePlayer();
	public:
		virtual void Start() override;
		virtual void Update() override;

		CameraFollowerComponent(float mapWidth, float mapHeight);
		~CameraFollowerComponent();
		CameraFollowerComponent(const CameraFollowerComponent& other) = delete;
		CameraFollowerComponent(CameraFollowerComponent&& other) = delete;
		CameraFollowerComponent& operator=(const CameraFollowerComponent& other) = delete;
		CameraFollowerComponent& operator=(CameraFollowerComponent&& other) = delete;

		virtual void OnNotify(const Event& event) override;
		virtual void OnSubjectDestroyed(Subject*) override {}

    };
}
