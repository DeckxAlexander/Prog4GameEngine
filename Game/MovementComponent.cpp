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



}

dae::MovementComponent::MovementComponent(GameObject* pOwner, float speed, GridComponent* pGrid) : ObjectComponent(pOwner), m_Speed{speed}, m_pGrid{pGrid}
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



    float snapSpeed{ 12.f };
    float tileSizeX{ m_pGrid->GetTileScale().x};
    float tileSizeY{ m_pGrid->GetTileScale().y};
    //Collisions Checks
    const auto& colliders = CollisionsManager::GetInstance().GetColliders();

    bool hasHit = false;
    //X
    if (velocity.x != 0)
    {
        pos.x += velocity.x;

        float targetY = std::round((pos.y - tileSizeY * 0.5f) / tileSizeY) * tileSizeY + tileSizeY * 0.5f;
        pos.y = glm::mix(pos.y, targetY, snapSpeed * deltaT);

        m_pOwner->SetPosition(pos.x, pos.y);


        for (auto* other : colliders)
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
                    float halfWidthA = a.z * 0.5f;
                    float halfWidthB = b.z * 0.5f;

                    float centerB = b.x + halfWidthB;

                    if (velocity.x > 0)
                    {
                        pos.x = centerB - halfWidthB - halfWidthA;
                    }
                    else if (velocity.x < 0)
                    {
                        pos.x = centerB + halfWidthB + halfWidthA;
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

                float halfWidthA = a.z * 0.5f;
                float halfWidthB = b.z * 0.5f;

                float centerB = b.x + halfWidthB;

                if (velocity.x > 0)
                {
                    pos.x = centerB - halfWidthB - halfWidthA;
                }
                else if (velocity.x < 0)
                {
                    pos.x = centerB + halfWidthB + halfWidthA;
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

        float targetX = std::round((pos.x - tileSizeX * 0.5f) / tileSizeX) * tileSizeX + tileSizeX * 0.5f;
        pos.x = glm::mix(pos.x, targetX, snapSpeed * deltaT);

        m_pOwner->SetPosition(pos.x, pos.y);


        for (auto* other : colliders)
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
                    float halfHeightA = a.w * 0.5f;
                    float halfHeightB = b.w * 0.5f;

                    float centerB = b.y + halfHeightB;

                    if (velocity.y > 0)
                    {
                        pos.y = centerB - halfHeightB - halfHeightA;
                    }
                    else if (velocity.y < 0)
                    {
                        pos.y = centerB + halfHeightB + halfHeightA;
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

                float halfHeightA = a.w * 0.5f;
                float halfHeightB = b.w * 0.5f;

                float centerB = b.y + halfHeightB;

                if (velocity.y > 0)
                {
                    pos.y = centerB - halfHeightB - halfHeightA;
                }
                else if (velocity.y < 0)
                {
                    pos.y = centerB + halfHeightB + halfHeightA;
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

dae::WanderMovementComponent::WanderMovementComponent(GameObject* pOwner, float speed, GridComponent* pGrid) : MovementComponent(pOwner, speed, pGrid)
{
}

void dae::WanderMovementComponent::Update()
{
    m_Velocity = m_DesiredVelocity;
    MovementComponent::Update();
}

glm::vec3 dae::WanderMovementComponent::FindNewDirection()
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

glm::vec3 dae::ChaseMovementComponent::FindDirection()
{

    auto targetPos = m_pGrid->WorldPosToTile(m_Target->GetWorldPosition());
    auto pos = m_pGrid->WorldPosToTile(GetOwner()->GetWorldPosition());
    glm::ivec2 diff = targetPos - pos;

    glm::ivec2 preferred;

    if (abs(diff.x) > abs(diff.y)) {
        preferred = (diff.x > 0) ? glm::ivec2(1, 0)
            : glm::ivec2(-1, 0);
    }
    else {
        preferred = (diff.y > 0) ? glm::ivec2(0, 1)
            : glm::ivec2(0, -1);
    }

    if (m_pGrid->GetGridLayout()[m_pGrid->GridToIndex(pos + preferred)] != GridComponent::GridValue::soft &&
        m_pGrid->GetGridLayout()[m_pGrid->GridToIndex(pos + preferred)] != GridComponent::GridValue::hard)
    {
        return {preferred.x, preferred.y, 0};
    }


    std::vector<glm::ivec2> dirs = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };


    for (auto& dir : dirs) {
        if (m_pGrid->GetGridLayout()[m_pGrid->GridToIndex(pos + preferred)] != GridComponent::GridValue::soft &&
            m_pGrid->GetGridLayout()[m_pGrid->GridToIndex(pos + preferred)] != GridComponent::GridValue::hard) {
            return { dir.x, dir.y, 0 };
        }
    }
    std::cout << "NONE\n";

    return glm::vec3();
}

void dae::ChaseMovementComponent::SetTarget(GameObject* target)
{
    m_Target = target;
    SetVelocity(FindDirection());


}


void dae::ChaseMovementComponent::Update()
{
    m_Velocity = m_DesiredVelocity;

    m_RecalculateTimer += TimeManager::GetInstance().GetDeltaTime();
    if (m_RecalculateTimer > m_RecalcMaxTime)
    {
        m_RecalculateTimer = 0.f;
        std::cout << "recalc";
        Recalculate();
    }

    MovementComponent::Update();
}

dae::ChaseMovementComponent::ChaseMovementComponent(GameObject* pOwner, float speed, GridComponent* pGrid) : MovementComponent(pOwner, speed, pGrid)
{
}
