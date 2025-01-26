#include "Settings.h"

#define INSERT_IN_MAP_IF_NOT_EXISTS(map, key, value) if (map.find(key) == map.end()) map[key] = value;

float Settings::SoundVolume = 1.0f;

Settings::Settings(Chimp::Engine& engine)
	: m_Engine(engine),
	m_Settings(Chimp::YAMLBlockParser::Parse(GAME_DATA_FOLDER + std::string("/Settings.yml")).Data)
{
	INSERT_IN_MAP_IF_NOT_EXISTS(m_Settings.Floats, "MusicVolume", 0.25f);
	m_Engine.GetMusicPlayer().SetVolumeRange(0.0f, m_Settings.Floats["MusicVolume"]);

	INSERT_IN_MAP_IF_NOT_EXISTS(m_Settings.Floats, "SoundEffectsVolume", 1.0f);
	SoundVolume = m_Settings.Floats["SoundEffectsVolume"];
}

Settings::~Settings()
{
	m_Engine.GetYAMLSerialiser().Write(m_Settings, GAME_DATA_FOLDER + std::string("/Settings.yml"));
}

void Settings::RenderUI()
{
	// Music
	if (ImGui::SliderFloat("Music Volume", &m_Settings.Floats["MusicVolume"], 0.0f, 1.0f)) {
		m_Engine.GetMusicPlayer().SetVolumeRange(0.0f, m_Settings.Floats["MusicVolume"]);
	}

	// Sound effects
	if (ImGui::SliderFloat("Sound Volume", &m_Settings.Floats["SoundEffectsVolume"], 0.0f, 1.0f)) {
		SoundVolume = m_Settings.Floats["SoundEffectsVolume"];
	}
}
