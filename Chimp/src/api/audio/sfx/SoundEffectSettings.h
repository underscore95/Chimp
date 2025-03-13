#pragma once

#include "api/audio/ISound.h"
#include "stdafx.h"
#include "Loggers.h"

namespace Chimp {
	class SoundEffectSettings  {
	public:

		// If not using chimp resource manager, make sure your paths are correct (might need to call MakeFilesRelativeTo)
		SoundEffectSettings(const std::vector<std::string>& soundFiles, int maxSimultaneousSounds, float minPitch, float maxPitch);

		// Make all files relative to the given path unless they are already absolute paths
		// see GetPathRelativeToFile
		void MakeFilesRelativeTo(std::string_view path);

		[[nodiscard]] const std::vector<std::string>& GetSoundFiles() const;

		[[nodiscard]] int GetMaxSimultaneousSounds() const;

		[[nodiscard]] float GetMinPitch() const;

		[[nodiscard]] float GetMaxPitch() const;

	private:
		std::vector<std::string> m_SoundFiles;
		int m_MaxSimultaneousSounds;
		float m_MinPitch;
		float m_MaxPitch;
	};
}