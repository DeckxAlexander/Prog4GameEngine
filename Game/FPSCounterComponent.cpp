#include "FPSCounterComponent.h"
#include <string>
#include <memory>
#include "TextComponent.h"
#include "TimeManager.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include "GameObject.h"

void dae::FPSCounterComponent::Update()
{
	float value = TimeManager::GetInstance().GetDeltaTime();
	const float smoothing = 0.05f; 

	if (m_smoothDelta == 0.f)
		m_smoothDelta = value;
	else
		m_smoothDelta += (value - m_smoothDelta) * smoothing;

	float fps = 1.f / m_smoothDelta;

	float fpsRounded = std::roundf((fps) * 10.f) / 10.f;


	if (fpsRounded >= m_FPS + 0.2f || fpsRounded <= m_FPS - 0.2f)
	{
		m_FPS = fpsRounded;
		std::stringstream ss;
		ss << std::fixed << std::setprecision(1) << fpsRounded;


		if (auto textComp = GetOwner()->GetComponentByType<TextComponent>()) textComp->SetText(ss.str());
	}
}

dae::FPSCounterComponent::FPSCounterComponent() : ObjectComponent(), m_smoothDelta{ 0.f }, m_FPS{}
{
	
}
