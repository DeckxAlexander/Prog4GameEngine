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

        HealthComponent() : ObjectComponent(),  m_Subject{std::make_unique<Subject>()}
        {
        }

        //Subject* GetSubject() { return m_Subject.get(); } 

        void ProcessDeath();


    private:
        std::unique_ptr<Subject> m_Subject;
    };
}