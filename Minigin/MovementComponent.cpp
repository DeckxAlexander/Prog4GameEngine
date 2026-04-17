#include "MovementComponent.h"
#include "CollisionComponent.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "CollisionsManager.h"
#include "GridComponent.h"
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

        if (CollisionComponent::CheckBlockingCollision(
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
            HitCollider();
        }
    }

    //Y
    pos.y += velocity.y;
    m_pOwner->SetPosition(pos.x, pos.y);


    for (auto* other : colliders)
    {
        if (other == collider) continue;

        if (CollisionComponent::CheckBlockingCollision(
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
            HitCollider();
        }
    }
    

    m_pOwner->SetPosition(pos.x, pos.y);



}

void dae::MovementComponent::Render() const
{
	//Empty
}

dae::AIMovementComponent::AIMovementComponent(GameObject* pOwner, float speed, GridComponent* pGrid) : MovementComponent(pOwner, speed), m_pGrid{pGrid}
{
}

void dae::AIMovementComponent::Update()
{
    m_Velocity = m_DesiredVelocity;
    MovementComponent::Update();
}

glm::vec3 dae::AIMovementComponent::FindNewDirection()
{

    if (m_pGrid == nullptr) return -m_DesiredVelocity;

    auto pos = GetOwner()->GetTransform()->GetPosition();

    int gridX = int(GridTransform::SnapToGrid(pos.x, m_pGrid->GetTileScale().x) / m_pGrid->GetTileScale().x);
    int gridY = int(GridTransform::SnapToGrid(pos.y, m_pGrid->GetTileScale().y) / m_pGrid->GetTileScale().y);

    auto layout = m_pGrid->GetGridLayout();


    std::vector<glm::vec3> possibleDirections{ {1,0,0},
        {0,1,0}, {-1,0,0}, {0,-1,0} };


    //Check Surrounding Tiles
    //Oposite should get prioritized
    auto desiredVel = -m_DesiredVelocity;
    int desiredGridX = gridX + int(desiredVel.x);
    int desiredGridY = gridY + int(desiredVel.y);
    int index = m_pGrid->GridToIndex(desiredGridX, desiredGridY);
    if (index <= layout.size() && layout[index] == GridComponent::GridValue::empty) return desiredVel;

    for (auto dir : possibleDirections) 
    {
        if (dir == m_DesiredVelocity || dir == -m_DesiredVelocity) continue;
        desiredVel = dir;
        desiredGridX = gridX + int(desiredVel.x);
        desiredGridY = gridY + int(desiredVel.y);
        index = m_pGrid->GridToIndex(desiredGridX, desiredGridY);
        if (index <= layout.size() && layout[index] == GridComponent::GridValue::empty) return desiredVel;
    }


    return -m_DesiredVelocity; //Incase no other direction works. Pick Negative



}