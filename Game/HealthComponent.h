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

        HealthComponent(GameObject* pOwner, int startHealth) : ObjectComponent(pOwner), m_Health{ startHealth },  m_Subject{std::make_unique<Subject>()}
        {
        }

        Subject* GetSubject() { return m_Subject.get(); }
        int GetHealth() { return m_Health; }
    private:
        int m_Health;
        std::unique_ptr<Subject> m_Subject;
    };
}