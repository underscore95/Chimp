#pragma once

#include "IAudioImporter.h"
#include "ISound.h"
#include "IListener.h"
#include "IAudio.h"

namespace Chimp {
	class AudioManager {
	public:
		AudioManager();

		[[nodiscard]] std::unique_ptr<ISound> LoadSound(const std::filesystem::path& path);

		[[nodiscard]] IListener& GetListener();

	private:
		std::unique_ptr<IAudioImporter> CreateImporter();
		std::unique_ptr<IListener> CreateListener();
		std::unique_ptr<IAudio> CreateAudio();

	private:
		std::unique_ptr<IAudioImporter> m_Importer;
		std::unique_ptr<IListener> m_Listener;
		std::unique_ptr<IAudio> m_Audio;
	};
}