#include "SoundSystem.h"

std::unique_ptr<dae::ISoundSystem> dae::SoundSystemLocator::_ss_instance = std::make_unique<NullSoundSystem>();