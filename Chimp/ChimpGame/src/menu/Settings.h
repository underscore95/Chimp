#pragma once

#include "Chimp.h"

class Settings {
public:
	Settings(Chimp::Engine& engine);
	~Settings();

	void RenderUI();

	static float SoundVolume;

private:
	Chimp::Engine& m_Engine;
	Chimp::YAMLBlock m_Settings;
};