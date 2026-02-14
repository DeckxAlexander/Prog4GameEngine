#pragma once
#include <string>
#include <memory>
#include "Transform.h"

namespace dae
{
	class Texture2D;
	class ObjectComponent //Abstract class
	{
		//These variables are kept incase object has Render capabilities
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture{};
	public:
		virtual void Update() = 0;
		virtual void Render() const = 0;

		virtual void SetTexture(const std::string& filename);
		virtual void SetPosition(float x, float y) ;

		ObjectComponent() = default;
		virtual ~ObjectComponent() = default; //!Q Can i do this?
		ObjectComponent(const ObjectComponent& other) = delete;
		ObjectComponent(ObjectComponent&& other) = delete;
		ObjectComponent& operator=(const ObjectComponent& other) = delete;
		ObjectComponent& operator=(ObjectComponent&& other) = delete;
	};
}