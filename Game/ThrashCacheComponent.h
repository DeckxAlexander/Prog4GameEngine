#pragma once
#include "ObjectComponent.h"
#include <iostream>
#include <vector>
#include <chrono>

namespace dae
{

	class ThrashCacheComponent final : public ObjectComponent
	{
		mutable int m_Samples{ 100 };
		mutable int m_SamplesInt{ 10 };
		mutable std::vector<float> m_TimingsInt{};
		mutable std::vector<float> m_Timings3D{};
		mutable std::vector<float> m_Timings3DAlt{};

	public:
		virtual void Update() override;
		virtual void Render() const override;

		void RenderImGuiGameObject() const;
		void RenderImGuiInt() const;

        static std::vector<float> ThrashCache(int samples) ;

        static std::vector<float> ThrashCacheAlt(int samples) ;
        static std::vector<float> ThrashCacheInt(int samples) ;

		ThrashCacheComponent(GameObject* pOwner);
		~ThrashCacheComponent() = default;
		ThrashCacheComponent(const ThrashCacheComponent& other) = delete;
		ThrashCacheComponent(ThrashCacheComponent&& other) = delete;
		ThrashCacheComponent& operator=(const ThrashCacheComponent& other) = delete;
		ThrashCacheComponent& operator=(ThrashCacheComponent&& other) = delete;
	};




}


