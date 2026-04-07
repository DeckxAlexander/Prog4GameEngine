#include "MovementComponent.h"
#include "CollisionComponent.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "CollisionsManager.h"
#include <iostream>


dae::MovementComponent::MovementComponent(GameObject* pOwner, float speed) : ObjectComponent(pOwner), m_Speed{speed}
{
}



void dae::MovementComponent::Update()
{

    float deltaT{ TimeManager::GetInstance().GetDeltaTime() };
    CollisionComponent* collider = GetOwner()->GetComponentByType<CollisionComponent>();
    

    glm::vec3 pos = GetOwner()->GetTransform()->GetPosition();
    glm::vec3 velocity = m_Velocity * m_Speed *deltaT;
    m_Velocity = { 0.f, 0.f, 0.f };


    if (!collider) { 
        pos.x += velocity.x;
        pos.y += velocity.y;
        m_pOwner->SetPosition(pos.x, pos.y);
        return; }

    //Collisions Checks
    const auto& colliders = CollisionsManager::GetInstance().GetColliders();

    //X
    pos.x += velocity.x;
    m_pOwner->SetPosition(pos.x, pos.y);


    for (auto* other : colliders)
    {
        if (other == collider) continue;

        if (CollisionComponent::CheckCollision(
            collider,
            other))
        {
            auto a = collider->GetCollisionRect();
            auto b = other->GetCollisionRect();

            if (velocity.x > 0)
            {
                pos.x = b.x - a.z;
            }
            else if (velocity.x < 0)
            {
                pos.x = b.x + b.z;
            }

            collider->SetCollisionRectDirty();
        }
    }

    //Y
    pos.y += velocity.y;
    m_pOwner->SetPosition(pos.x, pos.y);


    for (auto* other : colliders)
    {
        if (other == collider) continue;

        if (CollisionComponent::CheckCollision(
            collider,
            other))
        {
            auto a = collider->GetCollisionRect();
            auto b = other->GetCollisionRect();

            if (velocity.y > 0)
            {
                pos.y = b.y - a.w;
            }
            else if (velocity.y < 0)
            {
                pos.y = b.y + b.w;
            }

            collider->SetCollisionRectDirty();
        }
    }
    

    m_pOwner->SetPosition(pos.x, pos.y);



}

void dae::MovementComponent::Render() const
{
	//Empty
}


