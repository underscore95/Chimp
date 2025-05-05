#include "api/utils/waves/WaveManager.h"
#include "Loggers.h"

namespace Chimp {

	WaveManager::Builder::Builder(Chimp::Engine& engine)
		: m_WaveManager(std::unique_ptr<WaveManager>(new WaveManager(engine))) {
	}

	WaveManager::Builder& WaveManager::Builder::AddWave(std::unique_ptr<Wave> wave) {
		m_WaveManager->m_Waves.push_back(std::move(wave));
		return *this;
	}

	std::unique_ptr<WaveManager> WaveManager::Builder::Build(float delayAfterWaves) {
		if (!m_WaveManager) {
			Loggers::WaveManager().Error("Attempted to build wave manager twice");
			return nullptr;
		}
		if (m_WaveManager->m_Waves.empty()) {
			Loggers::WaveManager().Warning("WaveManager created with no waves");
		}

		for (auto& wave : m_WaveManager->m_Waves) {
			if (wave == nullptr) {
				Loggers::WaveManager().Error("WaveManager created with a null wave");
			}
			else if (wave->m_Commands.empty()) {
				Loggers::WaveManager().Warning("WaveManager created with a wave with no commands");
			}
			wave->Delay(delayAfterWaves); // Delay after each wave
		}
		m_WaveManager->m_Waves.shrink_to_fit();
		return std::move(m_WaveManager);
	}

	WaveManager::WaveManager(Chimp::Engine& engine)
		: m_Engine(engine) {
	}

	void WaveManager::Update()
	{
		if (m_WaveFinished && m_CanStartNextWave) {
			m_CurrentWaveIndex++;
			m_WaveFinished = false;
			m_CanStartNextWave = m_WaveAutoStart;
			Loggers::WaveManager().Info("Starting wave " + std::to_string(m_CurrentWaveIndex));
		}
		if (!HasStartedFirstWave()) {
			return;
		}
		if (m_WaveFinished) {
			return;
		}
		if (AreAllWavesFinished()) {
			return;
		}

		auto& wave = m_Waves[m_CurrentWaveIndex];
		if (wave->Update(m_Engine)) {
			m_WaveFinished = true;
			for (auto& callback : m_WaveFinishedCallbacks) {
				callback(m_CurrentWaveIndex);
			}
		}
	}

	void WaveManager::AllowStartNextWave()
	{
		m_CanStartNextWave = true;
	}

	void WaveManager::AddWaveFinishedCallback(std::function<void(unsigned int)> callback)
	{
		m_WaveFinishedCallbacks.push_back(callback);
	}

	long long WaveManager::GetCurrentWaveIndex() const
	{
		return m_CurrentWaveIndex;
	}

	size_t WaveManager::GetWave() const {
		return m_CurrentWaveIndex + 1;
	}

	size_t WaveManager::NumWaves() const {
		return m_Waves.size();
	}

	bool WaveManager::IsCurrentWaveFinished() const
	{
		return m_WaveFinished || AreAllWavesFinished();
	}

	bool WaveManager::AreAllWavesFinished() const
	{
		return m_CurrentWaveIndex >= m_Waves.size();
	}

	bool WaveManager::HasStartedFirstWave() const
	{
		return m_CurrentWaveIndex >= 0;
	}

	void WaveManager::SetWaveAutoStart(bool autoStart)
	{
		m_WaveAutoStart = autoStart;
		m_CanStartNextWave |= autoStart;
	}
}