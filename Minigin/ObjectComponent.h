#pragma once
#include <string>
#include <memory>
#include "Transform.h"

namespace dae
{
	class Texture2D;
	class ObjectComponent //Abstract class
	{
	protected:
		//These variables are kept incase object has Render capabilities
		std::string m_ComponentName;

		Transform m_transform{};
	public:
		virtual void Update() = 0;
		virtual void Render() const = 0;

		virtual void SetPosition(float x, float y) ;
		void SetScale(float x, float y);

		std::string GetName() const;

		ObjectComponent(const std::string&	 componentName);
		virtual ~ObjectComponent() = default; //!Q Can i do this?
		ObjectComponent(const ObjectComponent& other) = delete;
		ObjectComponent(ObjectComponent&& other) = delete;
		ObjectComponent& operator=(const ObjectComponent& other) = delete;
		ObjectComponent& operator=(ObjectComponent&& other) = delete;
	};
}