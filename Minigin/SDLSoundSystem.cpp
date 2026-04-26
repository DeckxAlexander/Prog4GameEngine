#include "SDLSoundSystem.h"
#include "SoundEventQueue.h"
#include <thread>
#include <atomic>
#include <unordered_map>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <iostream>



namespace dae {
    struct SDLSoundSystem::Impl
    {
    public:
        Impl()
            : m_Running(true),
            m_Worker(&Impl::Process, this), m_Mixer{}
        {
            MIX_Init();
            SDL_AudioSpec spec{};
            spec.freq = 48000;
            spec.format = SDL_AUDIO_F32;
            spec.channels = 2;

            m_Mixer = MIX_CreateMixer(&spec);
        }

        ~Impl()
        {
            m_Running = false;
            m_Queue.Push({ 0 }); // wake thread
            m_Worker.join();

            for (auto& [id, data] : m_Sounds)
                SDL_free(data);
        }

        void Enqueue(const uint32_t soundId)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Queue.Push({ soundId });
        }

    private:
        void Process()
        {
            while (m_Running)
            {
                AudioEvent event;
                m_Queue.Pop(event);

                if (!m_Running) break;

                Play(event.SoundId);
            }
        }

        void Play(uint32_t id)
        {

            MIX_PlayTrack(m_Sounds[id], 0);
        }

        void LoadSound(const uint32_t id, const std::string& path)
        {
            auto audio = MIX_LoadAudio(m_Mixer, path.c_str(), false);
            m_Sounds[id] = MIX_CreateTrack(m_Mixer);
            MIX_SetTrackAudio(m_Sounds[id], audio);

        }

    private:
        std::atomic<bool> m_Running;
        std::thread m_Worker;
        std::mutex m_Mutex;
        AudioEventQueue m_Queue;
        MIX_Mixer* m_Mixer;
        std::unordered_map<uint32_t, MIX_Track*> m_Sounds;
    };

    SDLSoundSystem::SDLSoundSystem() : m_Impl{std::make_unique<Impl>()}
    {
    }


    SDLSoundSystem::~SDLSoundSystem() = default;
    void SDLSoundSystem::PlaySound(const uint32_t id, const float volume)
    {
        m_Impl->Enqueue(id);
    }

    void SDLSoundSystem::LoadSound(const uint32_t id, const std::string& path)
    {
        m_Impl->Enqueue(id);
    }
}