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
			m_text = std::to_string(1/TimeManager::GetInstance().GetDeltaTime());
			std::cout << TimeManager::GetInstance().GetDeltaTime() << "\n";
			m_needsUpdate = true;
			TextObject::Update();
		};

		FPSCounterComponent(const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 }) : TextObject(text, font, color)
		{}
		virtual ~FPSCounterComponent() = default;
		FPSCounterComponent(const FPSCounterComponent& other) = delete;
		FPSCounterComponent(FPSCounterComponent&& other) = delete;
		FPSCounterComponent& operator=(const FPSCounterComponent& other) = delete;
		FPSCounterComponent& operator=(FPSCounterComponent&& other) = delete;
	};
}