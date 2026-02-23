#pragma once
#include <string>
#include <memory>
#include "TextComponent.h"
#include "TimeManager.h"
#include <iostream>

namespace dae 
{
	class FPSCounterComponent final : public ObjectComponent
	{
	public:
		void Update() override 
		{

			float value = TimeManager::GetInstance().GetDeltaTime();


			// Exponential smoothing on delta time
			const float smoothing = 0.05f; // 0.05 = smoother, 0.15 = more responsive

			if (m_smoothDelta == 0.f)
				m_smoothDelta = value;
			else
				m_smoothDelta += (value - m_smoothDelta) * smoothing;

			// Now calculate FPS from smoothed delta
			float fps = 1.f / m_smoothDelta;

			float fpsRounded = std::roundf((fps) * 10.f) / 10.f;


			if (fpsRounded >= m_FPS+0.3f || fpsRounded <= m_FPS -0.3f)
			{
				m_FPS = fpsRounded;
				std::stringstream ss;
				ss << std::fixed << std::setprecision(1) << fpsRounded;


				if (auto textComp = m_pOwner->GetComponentByType<TextComponent>()) textComp->SetText(ss.str());
			}
		
		}

		void Render() const override 
		{
		}

		FPSCounterComponent(GameObject* pOwner) : ObjectComponent(pOwner), m_smoothDelta{0.f}, m_FPS{}
		{}
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