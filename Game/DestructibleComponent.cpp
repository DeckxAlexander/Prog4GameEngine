#include "DestructibleComponent.h"
#include "GameObject.h"

void dae::DestructibleComponent::DestroyObject()
{
    GetOwner()->MarkForDelete();
}

