#include "audio/sfx/SoundEffectSettings.h"
#include "api/utils/FilePath.h"

namespace Chimp {
	SoundEffectSettings::SoundEffectSettings(const std::vector<std::string>& soundFiles, 
		int maxSimultaneousSounds, 
		float minPitch, 
		float maxPitch) : 
		m_SoundFiles(soundFiles),
		m_MaxSimultaneousSounds(maxSimultaneousSounds),
		m_MinPitch(minPitch),
		m_MaxPitch(maxPitch)
	{
	}

	void SoundEffectSettings::MakeFilesRelativeTo(std::string_view path)
	{
		for (size_t i = 0; i < m_SoundFiles.size(); i++) {
			m_SoundFiles[i] = GetPathRelativeToFile(m_SoundFiles[i], path);
		}
	}

	const std::vector<std::string>& SoundEffectSettings::GetSoundFiles() const
	{
		return m_SoundFiles;
	}

	int SoundEffectSettings::GetMaxSimultaneousSounds() const
	{
		return m_MaxSimultaneousSounds;
	}

	float SoundEffectSettings::GetMinPitch() const
	{
		return m_MinPitch;
	}

	float SoundEffectSettings::GetMaxPitch() const
	{
		return m_MaxPitch;
	}
}