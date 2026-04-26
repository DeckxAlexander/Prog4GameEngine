#pragma once
#include "SoundSystem.h"
#include <memory>

namespace dae {
    class SDLSoundSystem final : public ISoundSystem
    {
    public:
        SDLSoundSystem();


        ~SDLSoundSystem();
        void PlaySound(const uint32_t id, const float volume) override;
        void LoadSound(const uint32_t id, const std::string& path) override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_Impl;
    };
}