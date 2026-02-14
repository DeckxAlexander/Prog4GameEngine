#pragma once
#include <string>
#include <functional>
#include <filesystem>
#include <chrono>

namespace dae
{
	class Minigin final
	{
		bool m_quit{};
		double m_TargetFPS{ 60.0 };
		std::chrono::duration<long long, std::milli> m_MsPerFrame{ long(1000.0 / m_TargetFPS) };
		std::chrono::high_resolution_clock::time_point m_LastTime{};
	public:
		explicit Minigin(const std::filesystem::path& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);
		void RunOneFrame();

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	};
}