#include "MovementComponent.h"
#include "CollisionComponent.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "CollisionsManager.h"
#include "GridComponent.h"
#include <iostream>


void dae::MovementComponent::Start()
{

     m_Collider = GetOwner()->GetComponentByType<CollisionComponent>();
     m_Colliders = CollisionsManager::GetInstance().GetColliders();



}

dae::MovementComponent::MovementComponent(GameObject* pOwner, float speed) : ObjectComponent(pOwner), m_Speed{speed}
{



}



void dae::MovementComponent::Update()
{
    if (m_Velocity.length() == 0) return;

    float deltaT{ TimeManager::GetInstance().GetDeltaTime() };
    glm::vec3 pos = GetOwner()->GetTransform()->GetPosition();
    glm::vec3 velocity = m_Velocity * m_Speed * deltaT;
    m_Velocity = { 0.f, 0.f, 0.f };


    if (m_Collider == nullptr) {
        pos.x += velocity.x;
        pos.y += velocity.y;
        m_pOwner->SetPosition(pos.x, pos.y);
        return;
    }

    //Collisions Checks
   /* const auto& colliders = CollisionsManager::GetInstance().GetColliders();*/

    bool hasHit = false;
    //X
    if (velocity.x != 0)
    {
        pos.x += velocity.x;
        m_pOwner->SetPosition(pos.x, pos.y);


        for (auto* other : m_Colliders)
        {
            if (other == m_Collider) continue;

            //Wall Check
            if (other->GetTag() == 'w') 
            {
                auto a = m_Collider->GetCollisionRect();
                auto b = other->GetCollisionRect();
                auto posCol = glm::vec3(b.x, b.y, 0.f);
                if ((posCol - pos).length() > 5.f) continue;
                if (CollisionComponent::CheckCollision(a,b)) 
                {
                    if (velocity.x > 0)
                    {
                        pos.x = b.x - a.z;
                    }
                    else if (velocity.x < 0)
                    {
                        pos.x = b.x + b.z;
                    }
                    hasHit = true;
                    HitCollider();
                }
                continue;

            }

            //Other Collision Check
            if (CollisionComponent::CheckBlockingCollision(
                m_Collider,
                other))
            {
                auto a = m_Collider->GetCollisionRect();
                auto b = other->GetCollisionRect();

                if (velocity.x > 0)
                {
                    pos.x = b.x - a.z;
                }
                else if (velocity.x < 0)
                {
                    pos.x = b.x + b.z;
                }

                HitCollider();
                hasHit = true;
            }
        }
    }

    //Y
    if (velocity.y != 0) 
    {
        pos.y += velocity.y;
        m_pOwner->SetPosition(pos.x, pos.y);


        for (auto* other : m_Colliders)
        {
            if (other == m_Collider) continue;

            //WallCheck
            if (other->GetTag() == 'w')
            {
                auto a = m_Collider->GetCollisionRect();
                auto b = other->GetCollisionRect();
                auto posCol = glm::vec3(b.x, b.y, 0.f);
                if ((posCol - pos).length() > 5.f) continue;
                if (CollisionComponent::CheckCollision(a, b))
                {
                    if (velocity.y > 0)
                    {
                        pos.y = b.y - a.w;
                    }
                    else if (velocity.y < 0)
                    {
                        pos.y = b.y + b.w;
                    }
                    hasHit = true;
                    HitCollider();
                }
                continue;

            }

            //Other Collision Check
            if (CollisionComponent::CheckBlockingCollision(
                m_Collider,
                other))
            {
                auto a = m_Collider->GetCollisionRect();
                auto b = other->GetCollisionRect();

                if (velocity.y > 0)
                {
                    pos.y = b.y - a.w;
                }
                else if (velocity.y < 0)
                {
                    pos.y = b.y + b.w;
                }

                HitCollider();
                hasHit = true;

            }
        }
    }
    

    if (hasHit) m_pOwner->SetPosition(pos.x, pos.y);

    




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
    if (index <= int(layout.size()) && layout[index] == GridComponent::GridValue::empty) return desiredVel;

    for (auto dir : possibleDirections) 
    {
        if (dir == m_DesiredVelocity || dir == -m_DesiredVelocity) continue;
        desiredVel = dir;
        desiredGridX = gridX + int(desiredVel.x);
        desiredGridY = gridY + int(desiredVel.y);
        index = m_pGrid->GridToIndex(desiredGridX, desiredGridY);
        if (index <= int(layout.size()) && layout[index] == GridComponent::GridValue::empty) return desiredVel;
    }


    return -m_DesiredVelocity; //Incase no other direction works. Pick Negative

   

}