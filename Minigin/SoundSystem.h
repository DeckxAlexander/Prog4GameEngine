#include <memory>
#include <cstdint>
#include <string>

namespace dae
{

	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() = default;
		virtual void PlaySound(const uint32_t id) = 0;
		virtual void LoadSound(const uint32_t id, const std::string& path) = 0;
		virtual void Destroy() = 0;

	};



	class SoundSystemLocator final
	{
	private:
		static std::unique_ptr<ISoundSystem> _ss_instance;
	public:
		static ISoundSystem& get_sound_system() { return *_ss_instance; }
		static void register_sound_system(std::unique_ptr<ISoundSystem>&& ss) { _ss_instance = std::move(ss); }
	};



}