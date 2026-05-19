#include "Transform.h"
#include "GameObject.h"
#include "CollisionComponent.h"




void dae::Transform::SetScale(float x, float y, float z)
{
	m_scale.x = x;
	m_scale.y = y;
	m_scale.z = z;
}

void dae::Transform::SetScale(const glm::vec3& scale)
{
	m_scale = scale;
}

void dae::Transform::SetLocalPosition(const glm::vec3& position)
{
	m_LocalPosition = position;
	SetPositionDirty();
}

const glm::vec3& dae::Transform::GetWorldPosition() 
{
	if (m_PositionIsDirty) UpdateWorldPosition();
	return m_WorldPosition;

}

void dae::Transform::SetPositionDirty() 
{
	m_PositionIsDirty = true;

	for (GameObject* child : m_pOwner->GetChildren()) 
	{
		child->GetTransform()->SetPositionDirty();
	}

	CollisionComponent* collisionComp = m_pOwner->GetComponentByType<CollisionComponent>();
	if (collisionComp != nullptr)
	{
		collisionComp->SetCollisionRectDirty();
	}

}

void dae::Transform::UpdateWorldPosition() 
{
	if (m_PositionIsDirty) 
	{
		GameObject* pParent = m_pOwner->GetParent();

		if(pParent == nullptr)
		{
			m_WorldPosition = m_LocalPosition;
		}
		else
		{
			m_WorldPosition = pParent->GetWorldPosition() + m_LocalPosition;
		}
	}
	m_PositionIsDirty = false;

}
