
#include "ObjectComponent.h"



namespace dae
{
	class LifetimeComponent final : public ObjectComponent
	{
	private:
		float m_LifeTime;
		float m_AccumulatedTime{};
	public:
		virtual void Update() override;
		virtual void Render() const override {}


		LifetimeComponent(float lifetime);
		~LifetimeComponent() = default;
		LifetimeComponent(const LifetimeComponent& other) = delete;
		LifetimeComponent(LifetimeComponent&& other) = delete;
		LifetimeComponent& operator=(const LifetimeComponent& other) = delete;
		LifetimeComponent& operator=(LifetimeComponent&& other) = delete;


	};
}
