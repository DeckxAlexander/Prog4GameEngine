#pragma once
#include <vector>
#include <algorithm>
#include <memory>
#include "ObjectComponent.h"

namespace dae
{
    class ObjectComponent;

    enum class EventType
    {
        PlayerDead,
        ScoreChanged,
        LivesChanged,
        OwnerDead


    };

    struct Event
    {
        EventType event;
        ObjectComponent* sender;

    };

    class Subject;
    class IObserver
    {
    public:
        IObserver() = default;
        virtual ~IObserver() = default;
        virtual void OnNotify(const Event& event) = 0;
        virtual void OnSubjectDestroyed(Subject* subject) = 0;

    };

    class Subject 
    {
    public:
        void AddObserver(IObserver* observer) 
        {
            m_Observers.push_back(observer);
        }
        void RemoveObserver(IObserver* observer) { m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end()); }

        void Notify(const Event& event) 
        {

            for (auto observer : m_Observers) 
            {
                if (observer != nullptr) {

                    observer->OnNotify(event);
                }
            }
        }

        ~Subject() 
        {
            for (auto observer : m_Observers)
            {
                if (observer != nullptr) {

                    observer->OnSubjectDestroyed(this);
                }
            }
        }

    private:
        std::vector<IObserver*> m_Observers;
    };
}