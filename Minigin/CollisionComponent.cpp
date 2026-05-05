#include "CollisionComponent.h"
#include "CollisionComponent.h"
#include "CollisionsManager.h"
#include "GameObject.h"
#include "Renderer.h"


glm::vec4 dae::CollisionComponent::GetCollisionRect() 
{
	if (m_CollisionRectIsDirty) UpdateCollisionRect();
	return m_CollisionRect;

}



void dae::CollisionComponent::UpdateCollisionRect() 
{
	auto pos = m_pOwner->GetWorldPosition();

	m_CollisionRect.x = pos.x - m_CollisionRect.z*0.5f;
	m_CollisionRect.y = pos.y - m_CollisionRect.w * 0.5f;

	m_CollisionRectIsDirty = false;
}

void dae::CollisionComponent::Render() const
{

    auto pos = m_pOwner->GetWorldPosition();
    pos.x -= m_CollisionRect.z/2.f;
    pos.y -= m_CollisionRect.w/2.f;

    Renderer::GetInstance().RenderLine(pos.x, pos.y, pos.x + m_CollisionRect.z, pos.y);
    Renderer::GetInstance().RenderLine(pos.x + m_CollisionRect.z, pos.y, pos.x + m_CollisionRect.z, pos.y + m_CollisionRect.w);
    Renderer::GetInstance().RenderLine(pos.x, pos.y + m_CollisionRect.w, pos.x + m_CollisionRect.z, pos.y + m_CollisionRect.w);
    Renderer::GetInstance().RenderLine(pos.x, pos.y , pos.x , pos.y + m_CollisionRect.w);
}

dae::CollisionComponent::CollisionComponent(GameObject* pOwner, float width, float height, char tag) : ObjectComponent(pOwner), m_Tag{tag}
{
	m_CollisionRect.z = width;
	m_CollisionRect.w = height;

    CollisionsManager::GetInstance().AddCollision(this);


}

dae::CollisionComponent::~CollisionComponent()
{


    CollisionsManager::GetInstance().RemoveCollision(this);


}


bool dae::CollisionComponent::CheckBlockingCollision(CollisionComponent* collisionA, CollisionComponent* collisionB)
{
    auto a = collisionA->GetCollisionRect();
    auto b = collisionB->GetCollisionRect();

    auto colABlockingTags = collisionA->m_BlockingTags;
    auto colBBlockingTags = collisionB->m_BlockingTags;


    if ((std::find(colBBlockingTags.begin(), colBBlockingTags.end(), collisionA->GetTag()) != colBBlockingTags.end()) 
        || (std::find(colABlockingTags.begin(), colABlockingTags.end(), collisionB->GetTag()) != colABlockingTags.end())) return CheckCollision(a, b);

    return false;

}






    

bool dae::CollisionComponent::CheckCollision(glm::vec4 collisionRectA, glm::vec4 collisionRectB)
{
    auto a = collisionRectA;
    auto b = collisionRectB;

    float leftA = a.x;
    float rightA = a.x + a.z;
    float topA = a.y;
    float bottomA = a.y + a.w;

    float leftB = b.x;
    float rightB = b.x + b.z;
    float topB = b.y;
    float bottomB = b.y + b.w;

    return (
        leftA < rightB &&
        rightA > leftB &&
        topA < bottomB &&
        bottomA > topB
        );
}