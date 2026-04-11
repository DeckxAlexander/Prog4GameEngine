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
        //Add Events types here
        PlayerDead,
        ScoreChanged,
        Win


    };

    struct Event
    {
        EventType event;
        ObjectComponent* sender;

    };

    class Observer
    {
    public:
        Observer() = default;
        virtual ~Observer() = default;
        virtual void OnNotify(const Event& event) = 0;

    };

    class Subject 
    {
    public:
        void AddObserver(Observer* observer) {
   


                m_Observers.push_back(observer);
            
        }
        void RemoveObserver(Observer* observer) { m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end()); }

        void Notify(const Event& event) 
        {

            for (auto observer : m_Observers) 
            {
                if (observer != nullptr) {

                    observer->OnNotify(event);
                }
            }
        }

    private:
        std::vector<Observer*> m_Observers;
    };
}