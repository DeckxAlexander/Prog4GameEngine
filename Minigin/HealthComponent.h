#pragma once
#include "ObjectComponent.h"
#include "EventManagers.h"

namespace dae {
    class HealthComponent final : public ObjectComponent {
    public:
        void Update() override
        {
        }

        void Render() const override
        {
        }

        HealthComponent(GameObject* pOwner, int startHealth) : ObjectComponent(pOwner),  m_Subject{new Subject()}, m_Health{startHealth}
        {
        }

        Subject* GetSubject() { return m_Subject; }

        void TakeDamage(int damage) {
            m_Health -= damage;


            Event e{ EventType::PlayerDead, this };
            m_Subject->Notify(e);
            
        }

        int GetHealth() { return m_Health; }

    private:
        int m_Health;
        Subject* m_Subject;
    };
}