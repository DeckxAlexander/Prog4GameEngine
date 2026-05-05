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
            m_Worker{}, m_Mixer{}, m_Device{}
        {
            MIX_Init();
            SDL_AudioSpec spec{};
            spec.freq = 48000;
            spec.format = SDL_AUDIO_F32;
            spec.channels = 2;

            m_Device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
            m_Mixer = MIX_CreateMixerDevice(m_Device, &spec);

            m_Worker = std::jthread([this](std::stop_token st) {
                Process(st);
                });
        }

        ~Impl()
        {
            m_Running = false;
            m_Worker.request_stop();
            m_Queue.Push({});

            m_Worker.join();

            MIX_StopAllTracks(m_Mixer, 0);
            m_Sounds.clear();


            //SDL_CloseAudioDevice(m_Device);
            //MIX_DestroyMixer(m_Mixer);
            //
            //MIX_Quit();

        }

        void Enqueue(const uint32_t soundId)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Queue.Push({ soundId });
        }

        void LoadSound(const uint32_t id, const std::string& path)
        {
            auto* audio = MIX_LoadAudio(m_Mixer, path.c_str(), false);
            auto* track = MIX_CreateTrack(m_Mixer);
            MIX_SetTrackAudio(track, audio);
            std::lock_guard lock(m_Mutex);
            m_Sounds[id] = track;

        }

    private:
        void Process(std::stop_token st)
        {
            while (!st.stop_requested())
            {
                AudioEvent event;
                m_Queue.Pop(event);
                if (st.stop_requested()) break;
                Play(event.SoundId);
                std::cout << "Play";
            }
        }

        void Play(uint32_t id)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            if (m_Running == false) return;


            MIX_PlayTrack(m_Sounds[id], 0);
        }


    private:
        std::atomic<bool> m_Running;
        std::jthread m_Worker;
        std::mutex m_Mutex;
        AudioEventQueue m_Queue;
        SDL_AudioDeviceID m_Device;
        MIX_Mixer* m_Mixer;
        std::unordered_map<uint32_t, MIX_Track*> m_Sounds;
    };

    SDLSoundSystem::SDLSoundSystem() : m_Impl{std::make_unique<Impl>()}
    {
    }


    SDLSoundSystem::~SDLSoundSystem() = default;
    void SDLSoundSystem::PlaySound(const uint32_t id)
    {
        m_Impl->Enqueue(id);
    }

    void SDLSoundSystem::LoadSound(const uint32_t id, const std::string& path)
    {
        m_Impl->LoadSound(id, path);
    }
}