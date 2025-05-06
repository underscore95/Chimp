#include "api/audio/AudioManager.h"

#ifdef CHIMP_LIBSNDFILE
#include "impl/audio/libsndfile/AudioImporter.h"
#endif

#ifdef CHIMP_OPENAL
#include "impl/audio/openal/Sound.h"
#include "impl/audio/openal/Listener.h"
#include "impl/audio/openal/Audio.h"
#endif

#include "Loggers.h"

namespace Chimp {
	AudioManager::AudioManager() :
		m_Importer(CreateImporter()),
		m_Listener(CreateListener()),
		m_Audio(CreateAudio())
	{
		if (m_Listener) {
			m_Listener->SetPosition({ 0.0f, 0.0f, 0.0f });
			m_Listener->SetVelocity({ 0.0f, 0.0f, 0.0f });
			m_Listener->SetOrientation({ 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f });
		}
	}
	
	std::unique_ptr<ISound> AudioManager::LoadSound(const std::filesystem::path& path)
	{
#ifdef CHIMP_OPENAL
		return std::make_unique<Sound>(m_Importer->Load(path));
#endif

		Loggers::Audio().Error("No audio library available!");
		assert(false);
		return nullptr;
	}

	IListener& AudioManager::GetListener()
	{
		return *m_Listener;
	}

	std::unique_ptr<IAudioImporter> AudioManager::CreateImporter()
	{
#ifdef CHIMP_LIBSNDFILE
		return std::make_unique<AudioImporter>();
#endif

		Loggers::Audio().Error("No audio importer available!");
		assert(false);
		return nullptr;
	}

	std::unique_ptr<IListener> AudioManager::CreateListener()
	{
#ifdef CHIMP_OPENAL
		return std::make_unique<Listener>();
#endif

		Loggers::Audio().Error("No audio library available!");
		assert(false);
		return nullptr;
	}

	std::unique_ptr<IAudio> AudioManager::CreateAudio()
	{
		std::unique_ptr<IAudio> audio = nullptr;
#ifdef CHIMP_OPENAL
		audio = std::make_unique<Audio>();
#endif

		if (audio) {
			assert(audio->IsValid());
			if (audio->IsValid()) {
				Loggers::Audio().Info("Initialised Audio (OpenAL) successfully!");
			}
			return std::move(audio);
		}

		Loggers::Audio().Error("No audio library available!");
		assert(false);
		return nullptr;
	}
}