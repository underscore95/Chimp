#include "BloonSpawner.h"
#include "networking\Networking.h"

BloonSpawner::BloonSpawner(Chimp::Engine& engine,
	BloonManager& opponentBloonManager,
	MoneyManager& moneyManager,
	Chimp::TaskScheduler& taskScheduler,
	Chimp::WaveManager& waveManager) :
	m_Engine(engine),
	m_OpponentBloonManager(opponentBloonManager),
	m_BloonSendsDisabledTime(0),
	m_MoneyManager(moneyManager),
	m_TaskScheduler(taskScheduler),
	m_WaveManager(waveManager)
{
}

void BloonSpawner::Update()
{
	if (m_BloonSendsDisabledTime > 0) {
		m_BloonSendsDisabledTime -= m_Engine.GetTimeManager().GetDeltaTime();
	}
}

void BloonSpawner::RenderUI()
{
	std::string name = "Bloons##";

	ImGui::SetCursorPos({ 75,75 });

	const bool bloonSendsDisabled = m_BloonSendsDisabledTime > 0;
	if (bloonSendsDisabled) {
		ImGui::BeginDisabled();
	}

	const int wave = m_WaveManager.GetWave();

#pragma region Bloon Buttons
	bool canNotAfford = !m_MoneyManager.HasMoney(20);
	if (wave > 1) {
		if (canNotAfford) {
			ImGui::BeginDisabled();
		}
		if (ImGui::Button("Red x5 (-$20) (+$1/8s)")) {
			m_BloonSendsDisabledTime += 2.5f;
			m_MoneyManager.RemoveMoney(20);
			m_MoneyManager.AddToIncome(1.0f);
			for (size_t i = 0; i < 5; i++) {
				SendBloon(Bloons::BloonType::RED, i * 0.5f);
			}
		}
		if (canNotAfford) {
			ImGui::EndDisabled();
		}
	}

	if (wave > 2) {
		canNotAfford = !m_MoneyManager.HasMoney(30);
		if (canNotAfford) {
			ImGui::BeginDisabled();
		}
		ImGui::SetCursorPos({ 75, 100 });
		if (ImGui::Button("Blue x4 (-$30) (+$1.4/8s)")) {
			m_BloonSendsDisabledTime += 2.0f;
			m_MoneyManager.RemoveMoney(30);
			m_MoneyManager.AddToIncome(1.4f);
			for (size_t i = 0; i < 4; i++) {
				SendBloon(Bloons::BloonType::BLUE, i * 0.5f);
			}
		}
		if (canNotAfford) {
			ImGui::EndDisabled();
		}
	}

	if (wave > 3) {
		canNotAfford = !m_MoneyManager.HasMoney(50);
		if (canNotAfford) {
			ImGui::BeginDisabled();
		}
		ImGui::SetCursorPos({ 75, 125 });
		if (ImGui::Button("Green x3 (-$50) (+$1.8/8s)")) {
			m_MoneyManager.RemoveMoney(50);
			m_MoneyManager.AddToIncome(1.8f);
			m_BloonSendsDisabledTime += 1.5f;
			for (size_t i = 0; i < 3; i++) {
				SendBloon(Bloons::BloonType::GREEN, i * 0.5f);
			}
		}
		if (canNotAfford) {
			ImGui::EndDisabled();
		}

		canNotAfford = !m_MoneyManager.HasMoney(200);
		if (canNotAfford) {
			ImGui::BeginDisabled();
		}
		ImGui::SetCursorPos({ 75, 150 });
		if (ImGui::Button("XMAS (-$200) (-$1/8s)")) {
			m_BloonSendsDisabledTime += 4.0f;
			m_MoneyManager.RemoveMoney(200);
			m_MoneyManager.RemoveFromIncome(1.0f);
			for (size_t i = 0; i < 5; i++) {
				SendBloon(Bloons::BloonType::RED, i + 0.25f);
				SendBloon(Bloons::BloonType::GREEN, i + 0.5f);
				SendBloon(Bloons::BloonType::RED, i + 0.75f);
			}
		}
		if (canNotAfford) {
			ImGui::EndDisabled();
		}
	}

	if (wave > 4) {
		canNotAfford = !m_MoneyManager.HasMoney(300);
		if (canNotAfford) {
			ImGui::BeginDisabled();
		}
		ImGui::SetCursorPos({ 75, 175 });
		if (ImGui::Button("Pink x10 (-$300) (-$2/8s)")) {
			m_BloonSendsDisabledTime += 2.0f;
			m_MoneyManager.RemoveMoney(300);
			m_MoneyManager.AddToIncome(-2);
			for (size_t i = 0; i < 10; i++) {
				SendBloon(Bloons::BloonType::PINK, i * 0.5f);
			}
		}
		if (canNotAfford) {
			ImGui::EndDisabled();
		}
	}

	if (wave > 6) {
		canNotAfford = !m_MoneyManager.HasMoney(350);
		if (canNotAfford) {
			ImGui::BeginDisabled();
		}
		ImGui::SetCursorPos({ 75, 200 });
		if (ImGui::Button("Red x50 (-$350) (+$0/8s)")) {
			m_BloonSendsDisabledTime += 1.0f;
			m_MoneyManager.RemoveMoney(350);
			for (size_t i = 0; i < 50; i++) {
				SendBloon(Bloons::BloonType::RED, i * 0.05f);
			}
		}
		if (canNotAfford) {
			ImGui::EndDisabled();
		}
	}

	if (wave > 6) {
		canNotAfford = !m_MoneyManager.HasMoney(125);
		if (canNotAfford) {
			ImGui::BeginDisabled();
		}
		ImGui::SetCursorPos({ 75, 225 });
		if (ImGui::Button("Black x2 (-$125) (+$0/8s)")) {
			m_BloonSendsDisabledTime += 0.25f;
			m_MoneyManager.RemoveMoney(125);
			SendBloon(Bloons::BloonType::BLACK, 0.25f);
			SendBloon(Bloons::BloonType::BLACK, 0.5f);
		}
		if (canNotAfford) {
			ImGui::EndDisabled();
		}
	}

	if (wave > 7) {
		canNotAfford = !m_MoneyManager.HasMoney(400);
		if (canNotAfford) {
			ImGui::BeginDisabled();
		}
		ImGui::SetCursorPos({ 75, 250 });
		if (ImGui::Button("Zebra x3 (-$300) (-$2/8s)")) {
			m_BloonSendsDisabledTime += 2.0f;
			m_MoneyManager.RemoveMoney(300);
			m_MoneyManager.AddToIncome(-2);
			for (size_t i = 0; i < 3; i++) {
				SendBloon(Bloons::BloonType::ZEBRA, i * 0.5f);
			}
		}
		if (canNotAfford) {
			ImGui::EndDisabled();
		}
	}

	if (wave > 8) {
		canNotAfford = !m_MoneyManager.HasMoney(800);
		if (canNotAfford) {
			ImGui::BeginDisabled();
		}
		ImGui::SetCursorPos({ 75, 275 });
		if (ImGui::Button("Rainbow x3 (-$800) (-$4/8s)")) {
			m_BloonSendsDisabledTime += 2.0f;
			m_MoneyManager.RemoveMoney(800);
			m_MoneyManager.AddToIncome(-4);
			for (size_t i = 0; i < 3; i++) {
				SendBloon(Bloons::BloonType::RAINBOW, i * 0.5f);
			}
		}
		if (canNotAfford) {
			ImGui::EndDisabled();
		}
	}
#pragma endregion

	if (bloonSendsDisabled) {
		ImGui::EndDisabled();
	}
}

void BloonSpawner::SendBloon(Bloons::BloonType bloonType, float delay)
{
	m_TaskScheduler.RunDelayedTask(delay, [this, bloonType]() {
		m_OpponentBloonManager.SpawnBloon(bloonType);

		ClientSpawnBloonPacket packet;
		packet.PacketType = Networking::CLIENT_BLOON_SEND;
		packet.BloonType = bloonType;

#ifndef DEBUG_AUTOSTART_WITH_1_PLAYER
		const auto opponentId = Networking::GetClient()->GetHandlers().CurrentMatchHandler->GetOpponentId();
		Networking::GetClient()->GetClient().SendPacketToClient(opponentId, packet);
#endif
		});
}
