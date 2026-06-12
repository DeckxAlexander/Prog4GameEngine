#include "ObjectComponent.h"


namespace dae 
{
	class FPSCounterComponent final : public ObjectComponent
	{
	public:
		void Update() override;


		FPSCounterComponent();
		~FPSCounterComponent() = default;
		FPSCounterComponent(const FPSCounterComponent& other) = delete;
		FPSCounterComponent(FPSCounterComponent&& other) = delete;
		FPSCounterComponent& operator=(const FPSCounterComponent& other) = delete;
		FPSCounterComponent& operator=(FPSCounterComponent&& other) = delete;
	private:

		float m_smoothDelta;
		float m_FPS;
	};
}