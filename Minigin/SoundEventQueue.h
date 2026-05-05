#pragma once
#include <cstdint>
#include <queue>
#include <mutex>
#include <condition_variable>


namespace dae {
    struct AudioEvent
    {
        uint32_t SoundId;
    };

    class AudioEventQueue
    {
    public:
        void Push(const AudioEvent& event)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Queue.push(event);
            m_Condition.notify_one();
        }

        bool Pop(AudioEvent& outEvent)
        {
            std::unique_lock<std::mutex> lock(m_Mutex);

            m_Condition.wait(lock, [this] { return !m_Queue.empty(); });

            outEvent = m_Queue.front();
            m_Queue.pop();
            return true;
        }

        bool IsEmpty() { return m_Queue.empty(); }

        void Clear() {
            std::lock_guard<std::mutex> lock(m_Mutex);
            while (!m_Queue.empty())
            {
                m_Queue.pop();
            }
        }

    private:
        std::queue<AudioEvent> m_Queue;
        std::mutex m_Mutex;
        std::condition_variable m_Condition;
    };
}