#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "ObjectComponent.h"

namespace dae
{
	class Texture2D;
	class RenderComponent final : public ObjectComponent
	{
	private:
		std::shared_ptr<Texture2D> m_texture{};
		bool m_RenderOnScreen{ false };
	public:
		virtual void Update() override;
		virtual void Render() const override;

		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<Texture2D> texture);
		void SetRenderOnScreen(bool value) { m_RenderOnScreen = value; }


		RenderComponent(GameObject* pOwner);
		RenderComponent(GameObject* pOwner, const std::string& filename);
		~RenderComponent() = default; 
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;
	};
}