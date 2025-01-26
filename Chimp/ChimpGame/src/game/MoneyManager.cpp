#include "MoneyManager.h"

MoneyManager::MoneyManager()
{
	m_IncomeTimer.Start();
}

void MoneyManager::AddMoney(int amount) {
	Money += amount;
}

void MoneyManager::RemoveMoney(int amount) {
	Money -= amount;
}

int MoneyManager::GetMoney() const {
	return Money;
}

bool MoneyManager::HasMoney(int amount) const {
	return Money >= amount;
}

bool MoneyManager::SafeRemove(int amount) {
	if (HasMoney(amount)) {
		RemoveMoney(amount);
		return true;
	}
	return false;
}

int MoneyManager::GetIncome() const {
	return Income;
}

void MoneyManager::AddToIncome(float amount) {
	Income += amount;
}

void MoneyManager::RemoveFromIncome(float amount) {
	Income -= amount;
}

void MoneyManager::Update()
{
	if (m_IncomeTimer.GetSecondsElapsed() >= IncomeInterval) {
		AddMoney(Income);
		m_IncomeTimer.Reset().Start();
	}
}