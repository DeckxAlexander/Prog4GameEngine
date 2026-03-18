#pragma once
#include <vector>
#include <algorithm>
namespace dae
{
    class ObjectComponent;

    enum class EventType
    {
        //Add Events types here
        PlayerDead


    };

    struct Event
    {
        EventType event;
        ObjectComponent* sender;

    };

    class Observer 
    {
    public:
        virtual ~Observer() = default;
        virtual void OnNotify(const Event& event) = 0;
    };

    class Subject 
    {
    public:
        void AddObserver(Observer* observer) {
            if (!observer) return;

            // prevent duplicates
            if (std::find(m_Observers.begin(), m_Observers.end(), observer) == m_Observers.end()) {
                m_Observers.push_back(observer);
            }
        }

        void RemoveObserver(Observer* observer) {
            m_Observers.erase(
                std::remove(m_Observers.begin(), m_Observers.end(), observer),
                m_Observers.end()
            );
        }

        void Notify(const Event& event) {

            for (auto* observer : m_Observers) 
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