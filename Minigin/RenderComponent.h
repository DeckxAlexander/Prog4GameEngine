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
		std::shared_ptr<Texture2D> m_texture{};
	public:
		virtual void Update() override;
		virtual void Render() const override;

		virtual void SetTexture(const std::string& filename);

		RenderComponent(const std::string& filename, const std::string& componentName = "renderComponent");
		virtual ~RenderComponent() = default; 
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;
	};
}