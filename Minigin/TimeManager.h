#pragma once
#include "Singleton.h"
#include <iostream>

namespace dae
{
	class TimeManager final : public Singleton<TimeManager>
	{
	public:
		double GetDeltaTime() const 
		{
			return m_DeltaTime;

		}
		void SetDeltaTime(double deltaT) 
		{
			m_DeltaTime = deltaT;
			//std::cout << m_DeltaTime << "\n";
		}
	private:
		friend class Singleton<TimeManager>;
		TimeManager() = default;
		double m_DeltaTime{};
	};
}