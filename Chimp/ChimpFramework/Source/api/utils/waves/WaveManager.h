#pragma once

#include "Wave.h"

namespace Chimp {
	// Wave manager contains a vector of waves, and handles updating waves, and starting new waves.
	// You cannot restart the waves without creating a new wave manager.
	class WaveManager {
	public:
		// Factory class for wave manager
		class Builder {
		public:
			Builder(Chimp::Engine& engine);
		public:
			// Add a wave to the wave manager
			Builder& AddWave(std::unique_ptr<Wave> wave);
			// Build the wave manager, don't attempt to use the builder after calling this
			std::unique_ptr<WaveManager> Build(float delayAfterWaves = 1.0f);
		private:
			std::unique_ptr<WaveManager> m_WaveManager;
		};
		friend class Builder;
	private:
		WaveManager(Chimp::Engine& engine);
	public:
		void Update();

		// Allow the wave manager to start the next wave once the current wave has finished
		void AllowStartNextWave();

		// Add a callback to be called when the wave has finished
		// callback - function to be called when the wave has finished, takes the wave index as a parameter
		void AddWaveFinishedCallback(std::function<void(unsigned int)> callback);

		// Get current wave index (0-indexed)
		// is -1 if no waves have been started
		long long GetCurrentWaveIndex() const;
		// Get current wave number (1-indexed)
		size_t GetWave() const;
		// Get total number of waves
		size_t NumWaves() const;
		// Returns true if the current wave is finished or if all waves are finished
		bool IsCurrentWaveFinished() const;
		// Returns true if all waves are finished
		bool AreAllWavesFinished() const;
		// Returns true if the first wave has started
		bool HasStartedFirstWave() const;
		// If true, don't need to call AllowStartNextWave to start the next wave
		// If false, need to call AllowStartNextWave to start the next wave
		void SetWaveAutoStart(bool autoStart);
	private:
		long long m_CurrentWaveIndex = -1;
		std::vector<std::unique_ptr<Wave>> m_Waves;
		Chimp::Engine& m_Engine;
		bool m_CanStartNextWave = false;
		bool m_WaveAutoStart = false;
		bool m_WaveFinished = true;
		std::vector<std::function<void(unsigned int)>> m_WaveFinishedCallbacks;
	};
}