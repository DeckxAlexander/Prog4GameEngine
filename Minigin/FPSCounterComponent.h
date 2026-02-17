#pragma once
#include <string>
#include <memory>
#include "TextObject.h"
#include "TimeManager.h"
#include <iostream>

namespace dae 
{
	class FPSCounterComponent : public TextObject
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
			std::stringstream ss;
			ss << std::fixed << std::setprecision(1) << fpsRounded;
			m_text = ss.str() + "FPS";
			

			

			m_needsUpdate = true;
			

			TextObject::Update();

			
		};

		FPSCounterComponent(const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 }, const std::string& componentName = "textBox") : TextObject(text, font, color, componentName), m_smoothDelta{0.f}
		{}
		virtual ~FPSCounterComponent() = default;
		FPSCounterComponent(const FPSCounterComponent& other) = delete;
		FPSCounterComponent(FPSCounterComponent&& other) = delete;
		FPSCounterComponent& operator=(const FPSCounterComponent& other) = delete;
		FPSCounterComponent& operator=(FPSCounterComponent&& other) = delete;
	private:

		float m_smoothDelta;
	};
}