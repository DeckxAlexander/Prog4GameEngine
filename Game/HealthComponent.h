#pragma once
#include "ObjectComponent.h"
#include "EventManagers.h"

namespace dae {
    class HealthComponent final : public ObjectComponent {
    public:
        void Update() override {}
        HealthComponent();
        void ProcessDeath();

        ~HealthComponent() = default;
        HealthComponent(const HealthComponent& other) = delete;
        HealthComponent(HealthComponent&& other) = delete;
        HealthComponent& operator=(const HealthComponent& other) = delete;
        HealthComponent& operator=(HealthComponent&& other) = delete;
    };
}